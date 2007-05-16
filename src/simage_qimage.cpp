#include <simage_qimage.h>

#include <qglobal.h>
#include <qimage.h>
#include <qstring.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#if QT_VERSION > 0x040000
#define QT4 1
#else // QT version >= 4.0.0
#define QT4 0
#endif // QT version < 4.0.0

#if QT4
#include <qimagereader.h>
#include <qimagewriter.h>
#endif // QT4

#define ERR_NO_ERROR          0
#define ERR_OPEN              1
#define ERR_MEM               2
#define ERR_QIMAGE_WRITE      5
#define ERR_UNSUPPORTED_WRITE 6

static int qimageerror = ERR_NO_ERROR;

int
simage_qimage_error(char * buffer, int buflen)
{
  switch (qimageerror) {
  case ERR_OPEN:
    strncpy(buffer, "QImage loader: Error opening file", buflen);
    break;
  case ERR_MEM:
    strncpy(buffer, "QImage loader: Out of memory error", buflen);
    break;
  case ERR_QIMAGE_WRITE:
    strncpy(buffer, "QImage saver: Internal QImage error", buflen);    
    break;
  case ERR_UNSUPPORTED_WRITE:
    strncpy(buffer, "QImage saver: Unsupported file format", buflen);    
    break;
  }
  return qimageerror;
}

int 
simage_qimage_identify(const char * ptr,
                       const unsigned char *header,
                       int headerlen)
{
#if QT4
  return ! QImageReader::imageFormat(QString(ptr)).isEmpty();
#else
  return QImage::imageFormat(QString(ptr)) != NULL;
#endif
}

unsigned char *
simage_qimage_load(const char * filename,
                   int * width_ret,
                   int * height_ret,
                   int * numComponents_ret)
{
  QImage image;
  if (image.load(filename)) {
    int w = image.width();
    int h = image.height();
    // Keep in 8-bits mode if that was what we read
    int c;
    if (image.depth() == 8 && image.isGrayscale()) 
      c = 1;
    else {
#if QT4
      c = image.hasAlphaChannel() ? 4 : 3;
      image = image.convertToFormat(image.hasAlphaChannel() ? 
                                    QImage::Format_ARGB32 : QImage::Format_RGB32);
#else
      image = image.convertDepth(32);
      c = image.hasAlphaBuffer() ? 4 : 3;
#endif
    }

    unsigned char *buffer = (unsigned char *)malloc(w*h*c);
    if (buffer == NULL) {
      qimageerror = ERR_MEM;
      return NULL;
    }
    
    if (c == 1) {
      for (int i = 0; i < h; i++) {
        memcpy(buffer + i*w, image.scanLine(h-(i+1)), w);
      }
    }
    else { // (c == 3 || c == 4)
      QRgb * bits = (QRgb*) image.bits();
      for (int y = 0; y < h; y++) {
        unsigned char * line = &buffer[c*w*(h-(y+1))];
        for (int x = 0; x < w; x++) {
          *line++ = qRed(*bits);
          *line++ = qGreen(*bits);
          *line++ = qBlue(*bits);
          if (c == 4) {
            *line++ = qAlpha(*bits);
          }
          bits++;
        }
      }
    }
    
    *width_ret = w;
    *height_ret = h;
    *numComponents_ret = c;
    return buffer;
  }
  qimageerror = ERR_OPEN;
  return NULL;
}

#define MAX_EXT_LEN 255

static int
qimage_set_save_format(const char * ext, char * buf)
{
  strncpy(buf, ext, MAX_EXT_LEN);
  buf[MAX_EXT_LEN] = 0;
  
  int i = 0;
  // convert to upper case
  while (buf[i] != 0) {
    buf[i] = toupper(buf[i]);
    i++;
  }
  
  // Qt specifies the jpg extension as JPEG
  if (strcmp(buf, "JPG") == 0) strcpy(buf, "JPEG");
  
#if QT4

  QList <QByteArray> list = QImageWriter::supportedImageFormats();
  const int n = list.size();
  for (int i = 0; i < n; i++) {
    if (strcmp(buf, list[i].constData()) == 0) return 1;
  }
  buf[0] = 0;
  return 0;

#else // QT4

  QStrList olist = QImage::outputFormats();
  const char * qtext = olist.getFirst();
  while (qtext) {
    if (strcmp(buf, qtext) == 0) return 1;
    qtext = olist.next(); 
  }
  // clear save format
  buf[0] = 0;
  return 0;

#endif // !QT4
}

char * 
simage_qimage_get_savers(void)
{
#if QT4

  QByteArray str;
  QList <QByteArray> list = QImageWriter::supportedImageFormats();
  const int n = list.size();
  int first = 1;
  for (int i = 0; i < n; i++) {
    if (!first) str += ",";
    first = 0;
    str += list[i];
  }
  if (first) return NULL;
  const char * asc = str.constData(); 
  int len = strlen(asc);
  char * dst = (char*) malloc(len+1);
  strcpy(dst, asc);
  return dst;

#else // QT4

  QString str;
  int first = 1;
  QStrList olist = QImage::outputFormats();
  const char * qtext = olist.getFirst();
  while (qtext) {
    if (!first) str += ",";
    first = 0;
    str += qtext;
    qtext = olist.next();
  }
  if (first) return NULL; // no savers available
  const char * asc = str.ascii();
  int len = strlen(asc);
  char * dst = (char*) malloc(len+1);
  strcpy(dst, asc);
  return dst;

#endif // !QT4  
}


int 
simage_qimage_save(const char * filename,
                   const unsigned char * bytes,
                   int width,
                   int height,
                   int numcomponents,
                   const char * filetypeext)
{
  char ext[MAX_EXT_LEN+1];  
  qimage_set_save_format(filetypeext, ext);  
  if (ext[0] == 0) {
    qimageerror = ERR_UNSUPPORTED_WRITE;
    return 0;
  }
  
#if QT4
  QImage image(width, height, numcomponents == 2 || numcomponents == 4 ? 
               QImage::Format_ARGB32 : QImage::Format_RGB32);
#else // QT4
  QImage image(width, height, 32);
  if (numcomponents == 2 || numcomponents == 4) image.setAlphaBuffer(TRUE);
  else image.setAlphaBuffer(FALSE);
#endif // !QT4
  QRgb * bits = (QRgb*) image.bits();
  
  for (int y = 0; y < height; y++) {
    const unsigned char * line = 
      &bytes[width*numcomponents*(height-(y+1))];
    for (int x = 0; x < width; x++) {
      switch (numcomponents) {
      default:
      case 1:
        *bits++ = qRgb(line[0], line[0], line[0]);
        break;
      case 2:
        *bits++ = qRgba(line[0], line[0], line[0], line[1]);
        break;
      case 3:
        *bits++ = qRgb(line[0], line[1], line[2]);
        break;
      case 4:
        *bits++ = qRgba(line[0], line[1], line[2], line[3]);
        break;
      }
      line += numcomponents;
    }
  }
  bool ret = image.save(QString(filename), ext);
  if (!ret) {
    qimageerror = ERR_QIMAGE_WRITE;
    return 0;
  }
  return 1;
}
