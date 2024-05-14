<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.10.0" doxygen_gitid="ebc57c6dd303a980bd19dd74b8b61c8f3f5180ca">
  <compound kind="file">
    <name>avi_encode.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>avi__encode_8h.html</filename>
    <member kind="function">
      <type>void *</type>
      <name>avi_begin_encode</name>
      <anchorfile>avi__encode_8h.html</anchorfile>
      <anchor>ad139cb33669d96280d129f881cf95214</anchor>
      <arglist>(const char *filename, int width, int height, int fps, const char *preferences_filename)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>avi_encode_bitmap</name>
      <anchorfile>avi__encode_8h.html</anchorfile>
      <anchor>a5bfc32f625422b4defbbb8ac8e6cd37a</anchor>
      <arglist>(void *handle, unsigned char *buf, int rgb2bgr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>avi_end_encode</name>
      <anchorfile>avi__encode_8h.html</anchorfile>
      <anchor>a59d14c6714195267ce89b031632b3554</anchor>
      <arglist>(void *handle)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage_8h.html</filename>
    <class kind="struct">simage_plugin</class>
    <member kind="define">
      <type>#define</type>
      <name>__SIMAGE__</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a85286f32d82f435d65e779fec2449883</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_VERSION_1_1</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a85779861678580b5ad79e0940b20be00</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_VERSION_1_2</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a2221667f3b7a1b72c0594289484491e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_VERSION_1_3</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>af88e48b0d7c45c49b4c94b173d682bdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_VERSION_1_4</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a07ab693016eda18e5b200bab096cea4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_VERSION_1_5</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>aad3fd044f7ecb17780b265d95994acef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_VERSION_1_6</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a67adc913941ae26db331635484c3ff5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>s_dlclose_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a97396ad706680a93154e4a8355f27616</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="typedef">
      <type>void *</type>
      <name>s_dlopen_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ade04225a3b9e996fd0e489d4d1ad8bbf</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="typedef">
      <type>void *</type>
      <name>s_dlsym_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a44e585a53a1d7fbd0c76ef5cf4e8f2a9</anchor>
      <arglist>(void *handle, const char *symbolname)</arglist>
    </member>
    <member kind="typedef">
      <type>struct simage_image_s</type>
      <name>s_image</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a4564bc3af8acd05ba56cba52c1deab01</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct simage_movie_s</type>
      <name>s_movie</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a711f060954a1831b7d401ccaeecc076b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>s_movie_close_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a7e532d141fd4d7bd6a325bd21b8ca895</anchor>
      <arglist>(s_movie *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_movie_create_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>aa6dbf10fe954597852c7a2465575fcaa</anchor>
      <arglist>(const char *, s_movie *, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>s_image *</type>
      <name>s_movie_get_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adf83816255f5925edaa37ca71c18387f</anchor>
      <arglist>(s_movie *, s_image *, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_movie_open_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ab832f3bd683857be4349bb4be1d40569</anchor>
      <arglist>(const char *, s_movie *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_movie_put_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a0f247cb48f9044da0ebb2a3c971c2e71</anchor>
      <arglist>(s_movie *, s_image *, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>struct simage_parameters_s</type>
      <name>s_params</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a0da0be3fd0e6b2de2b4016fa3ff703b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct simage_stream_s</type>
      <name>s_stream</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a98ffc39b161acb533ad58550daf14666</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>s_stream_close_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a2315e1071141b671b4b86240a32607b5</anchor>
      <arglist>(s_stream *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_stream_create_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a4e8e7655b63ad10761f5df402331a640</anchor>
      <arglist>(const char *, s_stream *, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>void *</type>
      <name>s_stream_get_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ab5b8ffd39a3b335f27a8d7eaf0d7e7c8</anchor>
      <arglist>(s_stream *, void *, int *, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_stream_open_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a6dc4bf4fd15403efead2844e90ccf50a</anchor>
      <arglist>(const char *, s_stream *, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_stream_put_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a1e44f24a8a60c7dd12c2f4941511a221</anchor>
      <arglist>(s_stream *, void *, int, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_stream_seek_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a5a6620f48e7f20a7a8d4d1dd06bd748a</anchor>
      <arglist>(s_stream *, int, int, s_params *)</arglist>
    </member>
    <member kind="typedef">
      <type>int</type>
      <name>s_stream_tell_func</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a995a29fa8e0af47bc311473bb6d55a9d</anchor>
      <arglist>(s_stream *, s_params *)</arglist>
    </member>
    <member kind="enumvalue">
      <name>S_INTEGER_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06ca2e7845ecbb977459f534d6a61f310a04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>S_BOOL_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06cae75cd3fa1b40c18a423f3b618d5d6125</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>S_FLOAT_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06ca628446bd623a85524ff43483f9532258</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>S_DOUBLE_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06ca30386014140ad1a58f041343f03396eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>S_STRING_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06caf2593ae6afafde82dc6e910adf890240</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>S_POINTER_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06ca32ca5964b0bf62bb1e13b92454677225</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>S_FUNCTION_PARAM_TYPE</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06ca2d13863b67006e4f47a9ec339dee55ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIMAGE_SEEK_SET</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a61dadd085c1777f559549e05962b2c9eaaa6802f04c3ac5f99ea2ed3218e22221</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIMAGE_SEEK_CUR</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a61dadd085c1777f559549e05962b2c9ea99ea84a92380627987b34df4bd1cdb68</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIMAGE_SEEK_END</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a61dadd085c1777f559549e05962b2c9ea97c6543c206d1f55f3748a630a8b0183</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIMAGE_ORDER_RGB</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635a4ceed2834146842ed3315ff55048c09d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIMAGE_ORDER_BGR</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635af1e7533af5b5e5c6b007a697c633f945</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_components</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>acdda2ae8150af5b5069a3b7f380ed480</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_image_create</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a0fad021ea5d28c83114182ab4c3ac799</anchor>
      <arglist>(int w, int h, int components, unsigned char *prealloc)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>s_image_data</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ab90419813f09ff21133b92ddae86784a</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_image_destroy</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a74d7dae14250b89f5cf6de49a9dbb38d</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_get_component_order</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ac4c8f31dedead0a125aa2786f00692fd</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_height</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ae063c1a68783c34e52fc7ff5f5e123c8</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_image_load</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ac2b222fe8803d0467882feb65c9f57dd</anchor>
      <arglist>(const char *filename, s_image *prealloc)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_image_open</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>adfbd93c0f3bc0cdf0dfcfab113b1128f</anchor>
      <arglist>(const char *file, int oktoreadall)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_read_line</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a26465fb19c4a10e1bf8846b3765a21ee</anchor>
      <arglist>(s_image *image, int line, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_save</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a508759261e9338af936b4c4a65f448c4</anchor>
      <arglist>(const char *filename, s_image *image, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_image_set</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a5104d7ebe08a8c9a3603a8cb5df6ba70</anchor>
      <arglist>(s_image *image, int w, int h, int components, unsigned char *data, int copydata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_set_component_order</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a6238ff7cc7ccf39c2ce223048536c89e</anchor>
      <arglist>(s_image *image, int order)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_width</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a6e7b5d788c3adb8cf9e23cbd3886f477</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_close</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a576124d738cbac82ee52127c123b8149</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>s_movie *</type>
      <name>s_movie_create</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a0061082b3db9f5cf3f6ebe5bb079380e</anchor>
      <arglist>(const char *filename, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_destroy</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a81e19050853db4cf6fc684ec02ab1b64</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_exporter_add</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>afa4db3c00f3a4942ab4e020c8d565190</anchor>
      <arglist>(s_movie_create_func *create, s_movie_put_func *put, s_movie_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_movie_get_image</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a2c23ef4fa81e919c0d5c76e82c5524ff</anchor>
      <arglist>(s_movie *movie, s_image *prealloc, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_importer_add</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a7d030b20de1ab12933f454f4b6a1da83</anchor>
      <arglist>(s_movie_open_func *open, s_movie_get_func *get, s_movie_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>s_movie *</type>
      <name>s_movie_open</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a4232c4223f756685fbcd743a91508554</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_movie_put_image</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a7dd2e499947b7212dbdec12b41ddfc96</anchor>
      <arglist>(s_movie *movie, s_image *image, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_params_copy</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a3d7a6a9812dd7bf36242b9eb5ab8c59b</anchor>
      <arglist>(s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_params_create</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a18281b949430df084c804702561d8f9c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_params_destroy</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a172abd7c7b966c20f68521b3758b4cae</anchor>
      <arglist>(s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_params_get</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a4451ca329866167dec57982ece0c7111</anchor>
      <arglist>(s_params *params,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_params_set</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ad895755ecd9e266935d7d8c6fb339d78</anchor>
      <arglist>(s_params *params,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_set_dynamic_loader_interface</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ad99f99334c82487a660a3ca199ecfb25</anchor>
      <arglist>(s_dlopen_func *dlopen, s_dlsym_func *dlsym, s_dlclose_func *dlclose)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_close</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a5b10cb26480c72bd93e7abcf9ebfd599</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>s_stream *</type>
      <name>s_stream_create</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ac085060e2a689e556a0158fbbf9aaca9</anchor>
      <arglist>(const char *filename, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_destroy</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a8da1765eab30549fdf48f70b5578f469</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_exporter_add</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a200f0531dd434d1c92f5476a4726a1bc</anchor>
      <arglist>(s_stream_create_func *create, s_stream_put_func *put, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_exporter_add_ex</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ab36f2344a823d1bdae7e73376192c75e</anchor>
      <arglist>(s_stream_create_func *create, s_stream_put_func *put, s_stream_seek_func *seek, s_stream_tell_func *tell, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>s_stream_get_buffer</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ac5a714c2f8ae1b211bf0b2c6ba817aa8</anchor>
      <arglist>(s_stream *stream, void *prealloc, int *size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_importer_add</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a310d04e7012e18033d2a3e51f0eb2dc1</anchor>
      <arglist>(s_stream_open_func *open, s_stream_get_func *get, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_importer_add_ex</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ad57d3643ce92f348f0d1588b9280b626</anchor>
      <arglist>(s_stream_open_func *open, s_stream_get_func *get, s_stream_seek_func *seek, s_stream_tell_func *tell, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>s_stream *</type>
      <name>s_stream_open</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ae814cc9ea5f238fff4dcfebe04cda99d</anchor>
      <arglist>(const char *filename, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_stream_params</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a1026c2ff2bb109f6b41e31785e73c255</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_stream_put_buffer</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a87a02f510adb825ba66bf42c5f704843</anchor>
      <arglist>(s_stream *stream, void *buffer, int size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_stream_seek</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a8324ab16a7d9675b41a145154c815a16</anchor>
      <arglist>(s_stream *stream, int offset, int whence, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_stream_tell</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ad24bb8cb2494052a24c7bf738528c151</anchor>
      <arglist>(s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_add_loader</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a9fc3eefc8e14d369ad0da246df31e810</anchor>
      <arglist>(const struct simage_plugin *l, int addbefore)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_add_saver</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a427ecb08032d50a7953c8cb5d9bd6388</anchor>
      <arglist>(int(*save_func)(const char *name, const unsigned char *bytes, int width, int height, int nc), int(*error_func)(char *textbuffer, int bufferlen), const char *extensions, const char *fullname, const char *description, int addbefore)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_check_save_supported</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a364baad234011c5f9b4d6af8dded0861</anchor>
      <arglist>(const char *filenameextension)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_check_supported</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ae7338911967bdbe8863314a1d6506566</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_free_image</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a74acd8c16c6bfffc790728c89f0aeaef</anchor>
      <arglist>(unsigned char *imagedata)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_last_error</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a475d570093b8da3228b38744b5494bce</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_get_num_savers</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a589cd048fba3039cb0df3ddf87fdef5d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_saver_description</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a63ea96a19be9763acee4c5d6138d6b78</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_saver_extensions</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a4fd40a1f66bef8c4b3bbb941f29fcfbe</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_saver_fullname</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>aa2de3b638b4c773c35ab486092452af7</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_get_saver_handle</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a1a40ff17227c793469a6de7c22c14214</anchor>
      <arglist>(int idx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_next_power_of_two</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a2c4ff74be4c1b86bb6cba351279ed46f</anchor>
      <arglist>(int val)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_read_image</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ae6f0c01ee49ddb3d83ba5c4f4846e1a8</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_remove_loader</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>aeaeb9093160b0d2b366c2c7c0e9d6186</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_remove_saver</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a4072566fefb5c7a34c5d8fcb00f66392</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_resize</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a9ffa841f0e66bf3ce8f662e6bb04b2a6</anchor>
      <arglist>(unsigned char *imagedata, int width, int height, int numcomponents, int newwidth, int newheight)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_resize3d</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>ab8ee49d924c7deab4859bb1d06ccfd80</anchor>
      <arglist>(unsigned char *imagedata, int width, int height, int numcomponents, int layers, int newwidth, int newheight, int newlayers)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_save_image</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a491e7af1ad67ee0959048d320a111bdb</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int w, int h, int numcomponents, const char *filenameextension)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_version</name>
      <anchorfile>simage_8h.html</anchorfile>
      <anchor>a5cc048c4afc7436a86f63802f5edd4bc</anchor>
      <arglist>(int *major, int *minor, int *micro)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_avi.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__avi_8h.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <member kind="function">
      <type>void</type>
      <name>avienc_movie_close</name>
      <anchorfile>simage__avi_8h.html</anchorfile>
      <anchor>ad722afb78897a36e6658ee8ee10dcbbb</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>avienc_movie_create</name>
      <anchorfile>simage__avi_8h.html</anchorfile>
      <anchor>a1e2d1bd1bbbc0cefada384a08122d745</anchor>
      <arglist>(const char *filename, s_movie *movie, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>avienc_movie_put</name>
      <anchorfile>simage__avi_8h.html</anchorfile>
      <anchor>a83c2d954e3e1cf3fb357b865f9fbdcb7</anchor>
      <arglist>(s_movie *movie, s_image *image, s_params *params)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_cgimage.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__cgimage_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_cgimage_error</name>
      <anchorfile>simage__cgimage_8h.html</anchorfile>
      <anchor>ae08d0e2d0248d7c5c4f8a3817cbbd213</anchor>
      <arglist>(char *textbuffer, int buffersize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_cgimage_get_savers</name>
      <anchorfile>simage__cgimage_8h.html</anchorfile>
      <anchor>a7ed1cf9a5d9a7fb173a8e3a495136e96</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_cgimage_identify</name>
      <anchorfile>simage__cgimage_8h.html</anchorfile>
      <anchor>ad58c94d538a54a873a97827733b8b6f8</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_cgimage_load</name>
      <anchorfile>simage__cgimage_8h.html</anchorfile>
      <anchor>a667e6e6b9ceccf7d869796091da59795</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_cgimage_save</name>
      <anchorfile>simage__cgimage_8h.html</anchorfile>
      <anchor>a1beecb7f8a4dc72c371aca7654d006a3</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *ext)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_eps.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__eps_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_eps_error</name>
      <anchorfile>simage__eps_8h.html</anchorfile>
      <anchor>aa5dd2423df0ebe2005e60a76393c0b4d</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_eps_save</name>
      <anchorfile>simage__eps_8h.html</anchorfile>
      <anchor>a1a1ca4ac23d0ea22431ac8de1e06a42d</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_gdiplus.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__gdiplus_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>simage_gdiplus_close</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a4e9dbe1db5873f60967bf113c6dad688</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_error</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>ab9c18920afeccacf82cd5acbd06b799e</anchor>
      <arglist>(char *textbuffer, int buffersize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_gdiplus_get_savers</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a14c0f43318365719213f6e33223977eb</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_identify</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a183cdd55bb65920578126ab3b7ef6f8a</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_gdiplus_load</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a0798468a9d3d5dc442a55c12a23145d3</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_gdiplus_open</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a199d6aceb14115bb275e4b9d618c29ba</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_read_line</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>ab4ec99d531c4a0d22e9dfecad4f31675</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_read_region</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a3d54d8ef89afcc75930617ce46cb3f0b</anchor>
      <arglist>(void *opendata, int x, int y, int w, int h, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_save</name>
      <anchorfile>simage__gdiplus_8h.html</anchorfile>
      <anchor>a9300676d0aa7e85bb23cac4f6cc8594b</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *ext)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_gif.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__gif_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_gif_error</name>
      <anchorfile>simage__gif_8h.html</anchorfile>
      <anchor>aa575b4bfda158edbd168610cdd2488a0</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gif_identify</name>
      <anchorfile>simage__gif_8h.html</anchorfile>
      <anchor>a6d50ffcc2c0e1ca3ce8d5a7cd9a812fa</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_gif_load</name>
      <anchorfile>simage__gif_8h.html</anchorfile>
      <anchor>aed21c96fd599e9a078537e081264e04c</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gif_save</name>
      <anchorfile>simage__gif_8h.html</anchorfile>
      <anchor>af02b323a0ad240b1ab46a9af432bce6e</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_jasper.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__jasper_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>simage_jasper_close</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>a6c6ec2e2d8b95006ac5c6ffb7e012936</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jasper_error</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>ab1675e2237a63a7232729bd97d531e58</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jasper_identify</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>a9e3f8b2ce2f41d2d7e68347015c75ba3</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_jasper_load</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>a2bab9a72d04c3bf427897d824254b983</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_jasper_open</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>abf1ccc737b76cf162be71ada5ed2f60e</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jasper_read_line</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>a9c151b02903a6c77a3ee0502be3da69a</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jasper_save</name>
      <anchorfile>simage__jasper_8h.html</anchorfile>
      <anchor>a64eac9be4d14688f40c699c628ba07b6</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_jpeg.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__jpeg_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_jpeg_error</name>
      <anchorfile>simage__jpeg_8h.html</anchorfile>
      <anchor>a0f797d071de42a81f3c88f4b92ff32b7</anchor>
      <arglist>(char *textbuffer, int buffersize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jpeg_identify</name>
      <anchorfile>simage__jpeg_8h.html</anchorfile>
      <anchor>ad70dbba05dfe2cba00a1718e771c12bd</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_jpeg_load</name>
      <anchorfile>simage__jpeg_8h.html</anchorfile>
      <anchor>aeacb8b5fe151775a5c3320a6265887a2</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jpeg_save</name>
      <anchorfile>simage__jpeg_8h.html</anchorfile>
      <anchor>a0cc5a2c0c06e5ea56a45d3379ab16e2e</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_libsndfile.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__libsndfile_8h.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <member kind="function">
      <type>void</type>
      <name>libsndfile_stream_close</name>
      <anchorfile>simage__libsndfile_8h.html</anchorfile>
      <anchor>a7f2f10a6ec3e8bcfbc16bb9675a8077a</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>libsndfile_stream_get</name>
      <anchorfile>simage__libsndfile_8h.html</anchorfile>
      <anchor>a0f3cdcb32257ae0854657f5a7a8e5b59</anchor>
      <arglist>(s_stream *stream, void *buffer, int *size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>libsndfile_stream_open</name>
      <anchorfile>simage__libsndfile_8h.html</anchorfile>
      <anchor>a1706e5153323abcbca4866dee14bc861</anchor>
      <arglist>(const char *filename, s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>libsndfile_stream_seek</name>
      <anchorfile>simage__libsndfile_8h.html</anchorfile>
      <anchor>af2f6330ed0bed0b046aed8a0513f43d6</anchor>
      <arglist>(s_stream *stream, int offset, int whence, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>libsndfile_stream_tell</name>
      <anchorfile>simage__libsndfile_8h.html</anchorfile>
      <anchor>ad5b018a3a4c7176e441d7a28c2ab5e7f</anchor>
      <arglist>(s_stream *stream, s_params *params)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_oggvorbis.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__oggvorbis_8h.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <member kind="function">
      <type>void</type>
      <name>oggvorbis_reader_stream_close</name>
      <anchorfile>simage__oggvorbis_8h.html</anchorfile>
      <anchor>afbdbc04e561ccb326e1df7e8608a21f1</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>oggvorbis_reader_stream_get</name>
      <anchorfile>simage__oggvorbis_8h.html</anchorfile>
      <anchor>a52d42ad48f509de86b9233c1b76eae77</anchor>
      <arglist>(s_stream *stream, void *buffer, int *size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>oggvorbis_reader_stream_open</name>
      <anchorfile>simage__oggvorbis_8h.html</anchorfile>
      <anchor>a115dcac5ab07fdf22aef78bc5aa9fd9f</anchor>
      <arglist>(const char *filename, s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>oggvorbis_reader_stream_seek</name>
      <anchorfile>simage__oggvorbis_8h.html</anchorfile>
      <anchor>a47ef0e98c899d54821940633c2befae6</anchor>
      <arglist>(s_stream *stream, int offset, int whence, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>oggvorbis_reader_stream_tell</name>
      <anchorfile>simage__oggvorbis_8h.html</anchorfile>
      <anchor>abb5a91a501450637560b316f12cc92ff</anchor>
      <arglist>(s_stream *stream, s_params *params)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_pic.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__pic_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_pic_error</name>
      <anchorfile>simage__pic_8h.html</anchorfile>
      <anchor>a155ef90fa18126b0e01aa7edc4cf6b79</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_pic_identify</name>
      <anchorfile>simage__pic_8h.html</anchorfile>
      <anchor>a2c1a9095113eb35441c804fb39eb26e8</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_pic_load</name>
      <anchorfile>simage__pic_8h.html</anchorfile>
      <anchor>a449e9ab329cc94f09ba0431f7213da38</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_png.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__png_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_png_error</name>
      <anchorfile>simage__png_8h.html</anchorfile>
      <anchor>a8e124441e17ca586b4f8b1c362bf71db</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_png_identify</name>
      <anchorfile>simage__png_8h.html</anchorfile>
      <anchor>a3ff48664d2e0bf2832d9eb15996f2600</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_png_load</name>
      <anchorfile>simage__png_8h.html</anchorfile>
      <anchor>a21689c42a291aa163a5b6d81f4381d47</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_png_save</name>
      <anchorfile>simage__png_8h.html</anchorfile>
      <anchor>a22195cd399db35a0aed4f879ae2c88c4</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_private.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__private_8h.html</filename>
    <includes id="simage_8h" name="simage.h" local="yes" import="no" module="no" objc="no">simage.h</includes>
    <class kind="struct">simage_image_s</class>
    <class kind="struct">simage_open_funcs</class>
    <member kind="function">
      <type>s_params *</type>
      <name>s_movie_params</name>
      <anchorfile>simage__private_8h.html</anchorfile>
      <anchor>a9a2b5667061accddc41373613ad765e4</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>s_stream_context_get</name>
      <anchorfile>simage__private_8h.html</anchorfile>
      <anchor>a91fc88fe1e7b5397bbf11983a0f64af7</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_context_set</name>
      <anchorfile>simage__private_8h.html</anchorfile>
      <anchor>ad9cafbd750ceb9b564a7afb9ddfdba06</anchor>
      <arglist>(s_stream *stream, void *context)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_qimage.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__qimage_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_qimage_error</name>
      <anchorfile>simage__qimage_8h.html</anchorfile>
      <anchor>ae7b26fa1707f57de71a384f5bb500850</anchor>
      <arglist>(char *textbuffer, int buffersize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_qimage_get_savers</name>
      <anchorfile>simage__qimage_8h.html</anchorfile>
      <anchor>a243086773c43179eaedf77a0fd080fe2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_qimage_identify</name>
      <anchorfile>simage__qimage_8h.html</anchorfile>
      <anchor>a07d3f50ce11c1901748fb09fd5832884</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_qimage_load</name>
      <anchorfile>simage__qimage_8h.html</anchorfile>
      <anchor>a632b9dd9a81037bde200bdf4077724b0</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_qimage_save</name>
      <anchorfile>simage__qimage_8h.html</anchorfile>
      <anchor>a51bce32f6da6a03d2d0d218b7fbcda8d</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *ext)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_quicktime.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__quicktime_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_quicktime_error</name>
      <anchorfile>simage__quicktime_8h.html</anchorfile>
      <anchor>aeef0c660ffd8e4672665fdc0e51bc382</anchor>
      <arglist>(char *textbuffer, int buffersize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_quicktime_get_savers</name>
      <anchorfile>simage__quicktime_8h.html</anchorfile>
      <anchor>abe6f147dc9791880f5c2776092083455</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_quicktime_identify</name>
      <anchorfile>simage__quicktime_8h.html</anchorfile>
      <anchor>a04e046d3950e11615df0a8821ba0048a</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_quicktime_load</name>
      <anchorfile>simage__quicktime_8h.html</anchorfile>
      <anchor>aa884817ca172a9b1dcdf50053f45dbd4</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_quicktime_save</name>
      <anchorfile>simage__quicktime_8h.html</anchorfile>
      <anchor>acd0bc358bfa6a52b1372db5ba9b5b1b6</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *ext)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_rgb.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__rgb_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>simage_rgb_close</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>a4b7afb05ca43f4f986bcf7b8eb9c72c4</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_error</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>a98f6165ac613be0b44cbc65cecf6526b</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_identify</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>a30d70b21fd0212c83202f365b0506e4e</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_rgb_load</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>a3a1ac4edc6ada9f5830a76c535b46ecb</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_rgb_open</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>ad554e3ab5ffa1e1a226f77bcee6aba70</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_read_line</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>a328d67939d1fb14590fedb3c1577faa2</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_save</name>
      <anchorfile>simage__rgb_8h.html</anchorfile>
      <anchor>a0905a80a5e9b4128d0ff8925cb1f6861</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_tga.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__tga_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_tga_error</name>
      <anchorfile>simage__tga_8h.html</anchorfile>
      <anchor>a40f0c744961a7d7e2730c41cb97feb69</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tga_identify</name>
      <anchorfile>simage__tga_8h.html</anchorfile>
      <anchor>ac50404ff2804072d1cb1090a936f4723</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_tga_load</name>
      <anchorfile>simage__tga_8h.html</anchorfile>
      <anchor>ab1af08e923d288ecbf3d64b46a8bb587</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_tiff.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__tiff_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>simage_tiff_close</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>a5a82b332de3f2df19266498ddcd28675</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_error</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>a4b8207e531dbe9a16f7619c54cc547cf</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_identify</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>accd24576fe58693b3c318c7f5e447eb6</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_tiff_load</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>a7f8516b440560f8e05114ccb2d96c9d3</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_tiff_open</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>a70081057f429a638e7e47e23db6156b1</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_read_line</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>ad874a9741dd6133549784ebc4cfbb19e</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_save</name>
      <anchorfile>simage__tiff_8h.html</anchorfile>
      <anchor>a86bd686f3a3052a2cf17b9d6ad2fa0b9</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_xwd.h</name>
    <path>/home/runner/work/simage/simage/include/</path>
    <filename>simage__xwd_8h.html</filename>
    <member kind="function">
      <type>int</type>
      <name>simage_xwd_error</name>
      <anchorfile>simage__xwd_8h.html</anchorfile>
      <anchor>a4360d59dea209da8cf12284c00c97411</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_xwd_identify</name>
      <anchorfile>simage__xwd_8h.html</anchorfile>
      <anchor>a9055f5767e411cdd700283e120bea998</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_xwd_load</name>
      <anchorfile>simage__xwd_8h.html</anchorfile>
      <anchor>a1013931b7bfae5776eb685ef27637946</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_xwd_save</name>
      <anchorfile>simage__xwd_8h.html</anchorfile>
      <anchor>a16e83ff4cb2b497660bfb7a7515c5e48</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>avi_encode.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>avi__encode_8c.html</filename>
  </compound>
  <compound kind="file">
    <name>movie.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>movie_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <class kind="struct">simage_movie_exporter</class>
    <class kind="struct">simage_movie_importer</class>
    <class kind="struct">simage_movie_s</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_internal_exporters</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>ad72ad815311e51e7c9101f1728241516</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_internal_importers</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>ae3cb0640bb0eab39e33041e12e857ac8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_close</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a576124d738cbac82ee52127c123b8149</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>s_movie *</type>
      <name>s_movie_create</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a0061082b3db9f5cf3f6ebe5bb079380e</anchor>
      <arglist>(const char *filename, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_destroy</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a81e19050853db4cf6fc684ec02ab1b64</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_exporter_add</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>afa4db3c00f3a4942ab4e020c8d565190</anchor>
      <arglist>(s_movie_create_func *create, s_movie_put_func *put, s_movie_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_movie_get_image</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a2c23ef4fa81e919c0d5c76e82c5524ff</anchor>
      <arglist>(s_movie *movie, s_image *prealloc, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_movie_importer_add</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a7d030b20de1ab12933f454f4b6a1da83</anchor>
      <arglist>(s_movie_open_func *open, s_movie_get_func *get, s_movie_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>s_movie *</type>
      <name>s_movie_open</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a4232c4223f756685fbcd743a91508554</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_movie_params</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a9a2b5667061accddc41373613ad765e4</anchor>
      <arglist>(s_movie *movie)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_movie_put_image</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a7dd2e499947b7212dbdec12b41ddfc96</anchor>
      <arglist>(s_movie *movie, s_image *image, s_params *params)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static struct simage_movie_exporter *</type>
      <name>exporters</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a4e7b1b4b8ba17e45303f4bb9d4ea43be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static struct simage_movie_importer *</type>
      <name>importers</name>
      <anchorfile>movie_8c.html</anchorfile>
      <anchor>a11b7a824321ee264b59c4c43c7b939c2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>params.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>params_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <class kind="struct">simage_param_data</class>
    <class kind="struct">simage_parameters_s</class>
    <member kind="typedef">
      <type>void</type>
      <name>s_generic_func</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>ae6356a9e230d993a431f1948681330f2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_double_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a4e4bd4e3f5d12a7c35af4611440b5eb9</anchor>
      <arglist>(s_params *params, const char *name, double val)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_float_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a9d46a4e7436b3716d6c14b529ad0823e</anchor>
      <arglist>(s_params *params, const char *name, float val)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_function_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>acd80527759cc8086183868d20b4e4963</anchor>
      <arglist>(s_params *params, const char *name, void(*val)())</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_integer_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a57d12a0619afd0c76253061b1b6998da</anchor>
      <arglist>(s_params *params, const char *name, int val)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_pointer_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>adee6659542b3ab71fd53e864b8393abe</anchor>
      <arglist>(s_params *params, const char *name, void *val)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_string_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a3dff7a0b7ce62805dd353fa0a49fcfa5</anchor>
      <arglist>(s_params *params, const char *name, const char *val)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static struct simage_param_data *</type>
      <name>find_param</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>aaaed6dc1ac4be140952ec78ff9cd5c22</anchor>
      <arglist>(s_params *params, const char *name, int type, int allocnew)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_params_copy</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a3d7a6a9812dd7bf36242b9eb5ab8c59b</anchor>
      <arglist>(s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_params_create</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a18281b949430df084c804702561d8f9c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_params_destroy</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a172abd7c7b966c20f68521b3758b4cae</anchor>
      <arglist>(s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_params_get</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>a4451ca329866167dec57982ece0c7111</anchor>
      <arglist>(s_params *params,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_params_set</name>
      <anchorfile>params_8c.html</anchorfile>
      <anchor>ad895755ecd9e266935d7d8c6fb339d78</anchor>
      <arglist>(s_params *params,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>resize.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>resize_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <class kind="struct">CLIST</class>
    <class kind="struct">CONTRIB</class>
    <class kind="struct">Image</class>
    <member kind="define">
      <type>#define</type>
      <name>B</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a111da81ae5883147168bbb8366377b10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_spline_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a38017c2803309725b93a788854ca5f3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>bell_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a79cec5ac69df8b45d913af3c1070649a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>box_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a721494220bb82eca920f62cfc35c9c94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>C</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>ac4cf4b2ab929bd23951a8676eeac086b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>filter_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a363d6009dc94082f1ae350bef855e883</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Lanczos3_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>ae6941325439848843fc000e306ab51c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>ae71449b1cc6e6250b91f539153a7a0d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Mitchell_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a5401a3b6ad286f7e61440e2d4863ad80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>triangle_support</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a5e3b7c6c9f1cad300344de1eb7c31b92</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>B_spline_filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>ac1a8b9871ef6770e8a589a09937fde31</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>bell_filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a7d9321934f34f76ea208868795afe6bd</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>box_filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a7fa4da1421648aea0b7141bcb56b198b</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>acae742c724b478fc78a44b2d5e95bcbd</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>get_column</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>accea8b786ab60806c3f9905aaaefefc2</anchor>
      <arglist>(unsigned char *column, Image *image, int x)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>get_row</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a1c27c8f27ea73bad93001e9d393e32b7</anchor>
      <arglist>(unsigned char *row, Image *image, int y)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>Lanczos3_filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>aba1f436dc69f878851ee10cd3ab9fd9b</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>Mitchell_filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a72bfe8ad9e65e165db1c7ffe53bfc8bd</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Image *</type>
      <name>new_image</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>af57bae763d0383c89a4606ae57832a8e</anchor>
      <arglist>(int xsize, int ysize, int bpp, unsigned char *data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>put_pixel</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>af4e4c806abc8dc6ada93fc35757c318f</anchor>
      <arglist>(Image *image, int x, int y, float *data)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_resize</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a2ef6d3d52b0dc7d605296c5fb8645639</anchor>
      <arglist>(unsigned char *src, int width, int height, int num_comp, int newwidth, int newheight)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>simage_resize_fast</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>a27483f3e33e0c92696fafac6e16eaa10</anchor>
      <arglist>(unsigned char *src, int width, int height, int num_comp, int newwidth, int newheight)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>sinc</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>ae8ed18ac9e98ceef97d1a7a1480a5f4b</anchor>
      <arglist>(float x)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>triangle_filter</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>ac8b8705616434428c6dc79aa6a478bd2</anchor>
      <arglist>(float t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>zoom</name>
      <anchorfile>resize_8c.html</anchorfile>
      <anchor>acca539e18a3129a98cee528822576026</anchor>
      <arglist>(Image *dst, Image *src, float(*filterf)(float), float fwidth)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <includes id="simage__jpeg_8h" name="simage_jpeg.h" local="no" import="no" module="no" objc="no">simage_jpeg.h</includes>
    <includes id="simage__png_8h" name="simage_png.h" local="no" import="no" module="no" objc="no">simage_png.h</includes>
    <includes id="simage__tga_8h" name="simage_tga.h" local="no" import="no" module="no" objc="no">simage_tga.h</includes>
    <includes id="simage__tiff_8h" name="simage_tiff.h" local="no" import="no" module="no" objc="no">simage_tiff.h</includes>
    <includes id="simage__pic_8h" name="simage_pic.h" local="no" import="no" module="no" objc="no">simage_pic.h</includes>
    <includes id="simage__rgb_8h" name="simage_rgb.h" local="no" import="no" module="no" objc="no">simage_rgb.h</includes>
    <includes id="simage__gif_8h" name="simage_gif.h" local="no" import="no" module="no" objc="no">simage_gif.h</includes>
    <class kind="struct">_loader_data</class>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_ERROR_BUFSIZE</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a9fe9523ba3a0f96d9c8fe8789e0b225e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct _loader_data</type>
      <name>loader_data</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a1ea3655001887f77c5f0e4e40b63924e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_internal_loaders</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a3c9e4392694558f3da708b57edeb2ebf</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>add_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a047608a6d46aeec1a17e10c431e38895</anchor>
      <arglist>(loader_data *loader, unsigned char *(*load_func)(const char *, int *, int *, int *), int(*identify_func)(const char *, const unsigned char *, int headerlen), int(*error_func)(char *, int), int is_internal, int addbefore)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>cnt_bits</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a8388b74caf29c769d7273b662edd090a</anchor>
      <arglist>(int val, int *highbit)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static loader_data *</type>
      <name>find_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>ac95724113f2d44aa70ca1ed8c22eae93</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_image_open</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>ad34b6884f5402ae40307ca521027624d</anchor>
      <arglist>(const char *filename, int oktoreadall)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_read_line</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a26465fb19c4a10e1bf8846b3765a21ee</anchor>
      <arglist>(s_image *image, int line, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_set_dynamic_loader_interface</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a696ab624ee138e25a91f18aa554f753c</anchor>
      <arglist>(s_dlopen_func *dlopen_func, s_dlsym_func *dlsym_func, s_dlclose_func *dlclose_func)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_add_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>aff2b8375fce13b4ae7312124ecffc5a0</anchor>
      <arglist>(const struct simage_plugin *plugin, int addbefore)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_check_supported</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>ae7338911967bdbe8863314a1d6506566</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_clear_error</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a877026ad5fbac5225aac13c290c0a72f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_free_image</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a74acd8c16c6bfffc790728c89f0aeaef</anchor>
      <arglist>(unsigned char *imagedata)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_last_error</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a475d570093b8da3228b38744b5494bce</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_next_power_of_two</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a2c4ff74be4c1b86bb6cba351279ed46f</anchor>
      <arglist>(int val)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_read_image</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>ab75a6ae9441e26b3f860b79f383a319d</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numComponents)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_remove_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>aeaeb9093160b0d2b366c2c7c0e9d6186</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_version</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a5cc048c4afc7436a86f63802f5edd4bc</anchor>
      <arglist>(int *major, int *minor, int *micro)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static s_dlclose_func *</type>
      <name>dlclose</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a51fdc954cfd61099a4aa4013979e1b88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static s_dlopen_func *</type>
      <name>dlopen</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>acf3f7c60c301e8f5c4e26d67b6629000</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static s_dlsym_func *</type>
      <name>dlsym</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>ae3a27d843354b3430fec19d54a47529a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data *</type>
      <name>first_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a32669f1a033243ea7afaaee0b5491c64</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>gif_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a5e36cb83e15e490ab31ebbe6ac699dd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>jpeg_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a7b80bb4ef6d28ecf313bc4999e10f1b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data *</type>
      <name>last_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a3b1dfda85b4d87e7753334ea2c1e3af5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>pic_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>afd1f5ae13241eb526d2f39761977a15d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>png_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>ab81601bfde1b5e8aa141fd5cd576320f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>rgb_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a06a41536dd625da1bdd3b23f865452b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>simage_error_msg</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>aa09dc1e7c41cf809869b9199bb9eae09</anchor>
      <arglist>[512+1]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>targa_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>af44bf04974b3f8958e8a613f482ebe60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static loader_data</type>
      <name>tiff_loader</name>
      <anchorfile>simage_8c.html</anchorfile>
      <anchor>a9237040cb8f445bf13265e443a51802f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage12.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage12_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <member kind="function">
      <type>int</type>
      <name>s_image_components</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>acdda2ae8150af5b5069a3b7f380ed480</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_image_create</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>a0fad021ea5d28c83114182ab4c3ac799</anchor>
      <arglist>(int w, int h, int components, unsigned char *prealloc)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>s_image_data</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>ab90419813f09ff21133b92ddae86784a</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_image_destroy</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>a74d7dae14250b89f5cf6de49a9dbb38d</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_get_component_order</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>ac4c8f31dedead0a125aa2786f00692fd</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_height</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>ae063c1a68783c34e52fc7ff5f5e123c8</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
    <member kind="function">
      <type>s_image *</type>
      <name>s_image_load</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>ac2b222fe8803d0467882feb65c9f57dd</anchor>
      <arglist>(const char *filename, s_image *prealloc)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_save</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>a508759261e9338af936b4c4a65f448c4</anchor>
      <arglist>(const char *filename, s_image *image, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_image_set</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>a5104d7ebe08a8c9a3603a8cb5df6ba70</anchor>
      <arglist>(s_image *image, int w, int h, int components, unsigned char *data, int copydata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_set_component_order</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>a6238ff7cc7ccf39c2ce223048536c89e</anchor>
      <arglist>(s_image *image, int order)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_image_width</name>
      <anchorfile>simage12_8c.html</anchorfile>
      <anchor>a6e7b5d788c3adb8cf9e23cbd3886f477</anchor>
      <arglist>(s_image *image)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage13.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage13_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_resize3d</name>
      <anchorfile>simage13_8c.html</anchorfile>
      <anchor>a88c7cff45cb2097dc16fd2021fa428df</anchor>
      <arglist>(unsigned char *src, int width, int height, int nc, int layers, int newwidth, int newheight, int newlayers)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_avi.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__avi_8c.html</filename>
  </compound>
  <compound kind="file">
    <name>simage_cgimage.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__cgimage_8c.html</filename>
    <includes id="simage__cgimage_8h" name="simage_cgimage.h" local="no" import="no" module="no" objc="no">simage_cgimage.h</includes>
    <member kind="enumvalue">
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a94c4ca8995ad36230ba79a109323a927</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_OPEN</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a7235954be32cd0166432c3f7a5d61bb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_READ</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a65270b9d4567b81d393face0166d32d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_MEM</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7aaaeafdbf3aecc46864daa3cd829c7e05</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a6f86f11a0684bb3bfa0539a5c10aa66d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_WRITE</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a7b2c2fd2edcf9479181e72ee6ef6fffc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_NOT_IMPLEMENTED</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a5bed3f42a08e195b307cf8622e913327</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_INIT</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7ad64d9c982833860d10f54b846f9c5e46</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static CGImageSourceRef</type>
      <name>create_image_source</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>ad9ce8a1a5f6e2e18df76ae8ffef85305</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_cgimage_error</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>ab2bfe1243bd6d40de020f5138287ba49</anchor>
      <arglist>(char *cstr, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_cgimage_get_savers</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>a7ed1cf9a5d9a7fb173a8e3a495136e96</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_cgimage_identify</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>a9a70224a0e9f87bc8c9e022aa3f481ff</anchor>
      <arglist>(const char *file, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_cgimage_load</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>a5e76a3e3d6dfb8536dfe2181f98052d6</anchor>
      <arglist>(const char *file, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_cgimage_save</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>a1beecb7f8a4dc72c371aca7654d006a3</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *ext)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>cgimageerror</name>
      <anchorfile>simage__cgimage_8c.html</anchorfile>
      <anchor>a7312c04839d77de22e40c4343bc9dc89</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_eps.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__eps_8c.html</filename>
    <includes id="simage__eps_8h" name="simage_eps.h" local="no" import="no" module="no" objc="no">simage_eps.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>ae162e35f35c095230d397bd9954bacd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROWLEN</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>aaa2808bc952575c1a865a12a1b9a4fb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>encode_ascii85</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>ac5e5854671db474a1538fa55c3c95202</anchor>
      <arglist>(const unsigned char *in, unsigned char *out)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flush_ascii85</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>a6d705f3d453b58dbb3e9f721d4073f20</anchor>
      <arglist>(FILE *fp, unsigned char *tuple, unsigned char *linebuf, int *tuplecnt, int *linecnt, int rowlen)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>output_ascii85</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>a360c8d6db5684254dfda098ffb1544be</anchor>
      <arglist>(FILE *fp, const unsigned char val, unsigned char *tuple, unsigned char *linebuf, int *tuplecnt, int *linecnt, int rowlen, int flush)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_eps_error</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>a763e0dab9883d6bf01425bec0973a2ac</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_eps_save</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>a5af86e0c442d0772519b44d6d435b5eb</anchor>
      <arglist>(const char *filename, const unsigned char *src, int width, int height, int nc)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>epserror</name>
      <anchorfile>simage__eps_8c.html</anchorfile>
      <anchor>afe34376fa4d93a310d8a876eb1ceb2ae</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_gdiplus.cpp</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__gdiplus_8cpp.html</filename>
    <includes id="simage__gdiplus_8h" name="simage_gdiplus.h" local="no" import="no" module="no" objc="no">simage_gdiplus.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>LOCKBITS_RECT_CAST</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>acad7b6fbd2fd9190c89726154a736a0f</anchor>
      <arglist>(arg)</arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a94c4ca8995ad36230ba79a109323a927</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_OPEN</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a7235954be32cd0166432c3f7a5d61bb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_READ</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a65270b9d4567b81d393face0166d32d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_MEM</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5aaaeafdbf3aecc46864daa3cd829c7e05</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a6f86f11a0684bb3bfa0539a5c10aa66d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_WRITE</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a7b2c2fd2edcf9479181e72ee6ef6fffc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_NOT_IMPLEMENTED</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a5bed3f42a08e195b307cf8622e913327</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_INIT</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5ad64d9c982833860d10f54b846f9c5e46</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>copy16bppGrayScaleBuffer</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a5700aab0a51f2ad3d0a884be7697996f</anchor>
      <arglist>(unsigned char *src, unsigned int width, unsigned int height, unsigned int stride)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>copy32bppGrayScaleBuffer</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a09d683b4d10f244dfc3446f38580935e</anchor>
      <arglist>(unsigned char *src, unsigned int width, unsigned int height, unsigned int stride)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>copyImageBuffer</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a8f53fcc0ba28f8b6228217f8900658a0</anchor>
      <arglist>(unsigned char *src, unsigned int width, unsigned int height, unsigned int numcomponents, unsigned int stride)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>gdiplus_init</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>af6a2d179cd7a444524ef96002c49e71c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>GetEncoderClsid</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>abbc06edcf037f0092e7dfee74b7dd7ce</anchor>
      <arglist>(const char *format, CLSID *pClsid)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Gdiplus::PixelFormat</type>
      <name>getReadFormat</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a2997e708b037b1dc4a6a43b901ac4a4a</anchor>
      <arglist>(Gdiplus::Bitmap &amp;bitmap, bool &amp;grayscale)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_gdiplus_close</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a4e9dbe1db5873f60967bf113c6dad688</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_error</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a8bd7a4cb3ef86fecd9438f8fa93bfd1b</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_gdiplus_get_savers</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a14c0f43318365719213f6e33223977eb</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_identify</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a165355f7493116257e098c8853250001</anchor>
      <arglist>(const char *ptr, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_gdiplus_load</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a0798468a9d3d5dc442a55c12a23145d3</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_gdiplus_open</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a199d6aceb14115bb275e4b9d618c29ba</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_read_line</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>ab4ec99d531c4a0d22e9dfecad4f31675</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_read_region</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a3d54d8ef89afcc75930617ce46cb3f0b</anchor>
      <arglist>(void *opendata, int x, int y, int w, int h, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gdiplus_save</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a85b6859c0f9e4a6123a2e7dc1332ccb6</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *filetypeext)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>gdipluserror</name>
      <anchorfile>simage__gdiplus_8cpp.html</anchorfile>
      <anchor>a3035e3695ef313f0e9914a02f2e894e2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_gif.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__gif_8c.html</filename>
    <includes id="simage__gif_8h" name="simage_gif.h" local="no" import="no" module="no" objc="no">simage_gif.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>FALSE</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>aa93f0eb578d23995850d61f7d61c55c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a94c4ca8995ad36230ba79a109323a927</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_OPEN</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a7235954be32cd0166432c3f7a5d61bb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_READ</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a65270b9d4567b81d393face0166d32d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_WRITE</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a7b2c2fd2edcf9479181e72ee6ef6fffc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_MEM</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04aaaeafdbf3aecc46864daa3cd829c7e05</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>decode_row</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>a91177f4eb97dee0e3a3a3125add6bd2f</anchor>
      <arglist>(GifFileType *giffile, unsigned char *buffer, unsigned char *rowdata, int x, int y, int len, int transparent)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gif_error</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>a5a68d8770c09a62378d9a71033bfd983</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gif_identify</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>a6d50ffcc2c0e1ca3ce8d5a7cd9a812fa</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_gif_load</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>a99e7788f2f0aff6310fb41df5e7f04a8</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_gif_save</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>af02b323a0ad240b1ab46a9af432bce6e</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>giferror</name>
      <anchorfile>simage__gif_8c.html</anchorfile>
      <anchor>adc74fd13624442e775eeef5682f2e0dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_jasper.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__jasper_8c.html</filename>
  </compound>
  <compound kind="file">
    <name>simage_jpeg.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__jpeg_8c.html</filename>
    <class kind="struct">my_error_mgr</class>
    <member kind="define">
      <type>#define</type>
      <name>ERR_JPEGLIB</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a1085adf6ead8ce145ce054f8cf6974b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_JPEGLIB_WRITE</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a441512ca21fe11eeae23d2a54813887c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>ae162e35f35c095230d397bd9954bacd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct my_error_mgr *</type>
      <name>my_error_ptr</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a17bb08c876725fc3608be875c457af71</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>copyScanline</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a5c9f5dd011546f9997118f5cd4095bf5</anchor>
      <arglist>(unsigned char *currPtr, unsigned char *from, int cnt)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>my_error_exit</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a0a245e2d88932dc01cfaeb117aca4a50</anchor>
      <arglist>(j_common_ptr cinfo)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jpeg_error</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>aa5d978b3eb9f77fda45452dcf8dc5763</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jpeg_identify</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>af2beeef7433f5a8e26f759199f3ae642</anchor>
      <arglist>(const char *ptr, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_jpeg_load</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>ad3b8d6ae421f279ad94c1069f61725db</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_jpeg_save</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>a0cc5a2c0c06e5ea56a45d3379ab16e2e</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>jpegerror</name>
      <anchorfile>simage__jpeg_8c.html</anchorfile>
      <anchor>ad4da95f3770aaa5aabdc7d02b8e4ddf9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_libsndfile.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__libsndfile_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <includes id="simage__libsndfile_8h" name="simage_libsndfile.h" local="no" import="no" module="no" objc="no">simage_libsndfile.h</includes>
    <class kind="struct">libsndfile_context</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>libsndfile_cleanup_context</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>ac464739fc746fab4cb336628d564b070</anchor>
      <arglist>(libsndfile_context *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>libsndfile_init_context</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>a36bd30396d04c30a07f794bfe8ecf3f7</anchor>
      <arglist>(libsndfile_context *context)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>libsndfile_stream_close</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>a7f2f10a6ec3e8bcfbc16bb9675a8077a</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>libsndfile_stream_get</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>a0f3cdcb32257ae0854657f5a7a8e5b59</anchor>
      <arglist>(s_stream *stream, void *buffer, int *size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>libsndfile_stream_open</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>a1706e5153323abcbca4866dee14bc861</anchor>
      <arglist>(const char *filename, s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>libsndfile_stream_seek</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>af2f6330ed0bed0b046aed8a0513f43d6</anchor>
      <arglist>(s_stream *stream, int offset, int whence, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>libsndfile_stream_tell</name>
      <anchorfile>simage__libsndfile_8c.html</anchorfile>
      <anchor>ad5b018a3a4c7176e441d7a28c2ab5e7f</anchor>
      <arglist>(s_stream *stream, s_params *params)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_oggvorbis_reader.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__oggvorbis__reader_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <includes id="simage__oggvorbis_8h" name="simage_oggvorbis.h" local="no" import="no" module="no" objc="no">simage_oggvorbis.h</includes>
    <class kind="struct">oggvorbis_reader_context</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>oggvorbis_reader_cleanup_context</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a7fb8e22525ff5ee78517a3a883e14c2d</anchor>
      <arglist>(oggvorbis_reader_context *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>oggvorbis_reader_close</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a443685b1df9e31c6f9d0fcc1b0777cf8</anchor>
      <arglist>(oggvorbis_reader_context *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>oggvorbis_reader_close_cb</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a66f20692c57ed6a9b355a9defee232ed</anchor>
      <arglist>(void *datasource)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>oggvorbis_reader_get_stream_info</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>ab7ef6a06df1e38bfde5cda76e438dfb5</anchor>
      <arglist>(oggvorbis_reader_context *context, int *channels, int *samplerate)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>oggvorbis_reader_init_context</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>ae9bf5f7f07657be1116dd19f44353f98</anchor>
      <arglist>(oggvorbis_reader_context *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>oggvorbis_reader_open</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>af4276d1f8bc787b79dabdd5176253c05</anchor>
      <arglist>(oggvorbis_reader_context **contextp, const char *filename)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>oggvorbis_reader_read</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a7d72155b8d946d0f723fb11a0c39754a</anchor>
      <arglist>(oggvorbis_reader_context *context, char *buffer, int size)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>oggvorbis_reader_read_cb</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a4026fd786b6f74507875150e39c7b2ee</anchor>
      <arglist>(void *ptr, size_t size, size_t nmemb, void *datasource)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>oggvorbis_reader_seek_cb</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a9ffb27d9e3838a1d7417ed519efca870</anchor>
      <arglist>(void *datasource, ogg_int64_t offset, int whence)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>oggvorbis_reader_stream_close</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>afbdbc04e561ccb326e1df7e8608a21f1</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>oggvorbis_reader_stream_get</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a52d42ad48f509de86b9233c1b76eae77</anchor>
      <arglist>(s_stream *stream, void *buffer, int *size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>oggvorbis_reader_stream_open</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a115dcac5ab07fdf22aef78bc5aa9fd9f</anchor>
      <arglist>(const char *filename, s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>oggvorbis_reader_stream_seek</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>a47ef0e98c899d54821940633c2befae6</anchor>
      <arglist>(s_stream *stream, int offset, int whence, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>oggvorbis_reader_stream_tell</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>abb5a91a501450637560b316f12cc92ff</anchor>
      <arglist>(s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static long</type>
      <name>oggvorbis_reader_tell_cb</name>
      <anchorfile>simage__oggvorbis__reader_8c.html</anchorfile>
      <anchor>ace6594ce8bd246fba97a9fc8567678fb</anchor>
      <arglist>(void *datasource)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_pic.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__pic_8c.html</filename>
    <includes id="simage__pic_8h" name="simage_pic.h" local="no" import="no" module="no" objc="no">simage_pic.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_MEMORY</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a3020b3e07b918dd57f396e81a47ea50e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_NO_ERROR</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>ace434a3a200d2d2a53f3e0d438e6abd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_READ_ERROR</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>aeeddd8dd5edff4cb8706bbedebefbcc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_READING_HEADER</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a4ee74faf50c3e6a70db3ea67c76f4354</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_READING_PALETTE</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a95ed470a4f8ab3c5b603cd07e25bb23d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>readint16</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a1775b541a28b0ef6c4bda76f9fbbc053</anchor>
      <arglist>(FILE *fp, int *res)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_pic_error</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a155ef90fa18126b0e01aa7edc4cf6b79</anchor>
      <arglist>(char *buffer, int bufferlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_pic_identify</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a25d229e4d08555e57ea232bb9a11a636</anchor>
      <arglist>(const char *ptr, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_pic_load</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a30be6c845db375e4b129d1d251905d3f</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>picerror</name>
      <anchorfile>simage__pic_8c.html</anchorfile>
      <anchor>a51a95397dd4e7f89080278bbf356a5bd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_png.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__png_8c.html</filename>
    <includes id="simage__png_8h" name="simage_png.h" local="no" import="no" module="no" objc="no">simage_png.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM_WRITE</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a638ef8373690f4a1dee1c1d5f045ed95</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>ae162e35f35c095230d397bd9954bacd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_PNGLIB</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a8604893957c7318bd4f28c16d3f8dc43</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_PNGLIB_WRITE</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>afb0436047999605110f8f5025ab0bde7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>err_callback</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a28e7cb0f2e67c115f0a1ea2872ca9674</anchor>
      <arglist>(png_structp ps, png_const_charp pc)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_png_error</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>aeeb0725895c68c7e90376d1329bb6f0a</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_png_identify</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>af895ae9846a8aa6cdc2b4607814fb682</anchor>
      <arglist>(const char *ptr, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_png_load</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a0cc4008de5157493b520bb4bf4890f9f</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_png_save</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a22195cd399db35a0aed4f879ae2c88c4</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_flush_cb</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>ab9751cda6f16deea94203026181b12ed</anchor>
      <arglist>(png_structp png_ptr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_read_cb</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a6f9b01ac027e323807cf18739cc4473e</anchor>
      <arglist>(png_structp png_ptr, png_bytep data, png_uint_32 length)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_write_cb</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a9a854bc2639f38be8d49c19fe0cc7743</anchor>
      <arglist>(png_structp png_ptr, png_bytep data, png_uint_32 length)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>warn_callback</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a2f0d66b7587fc36d00fa5664dbc9e404</anchor>
      <arglist>(png_structp ps, png_const_charp pc)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>pngerror</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a99bf68e7f129cc7a38cc75bdad2a3578</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static jmp_buf</type>
      <name>setjmp_buffer</name>
      <anchorfile>simage__png_8c.html</anchorfile>
      <anchor>a5be06c604b7002679fcf9aee7d417072</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_qimage.cpp</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__qimage_8cpp.html</filename>
    <includes id="simage__qimage_8h" name="simage_qimage.h" local="no" import="no" module="no" objc="no">simage_qimage.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_QIMAGE_WRITE</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a4c66387afd4aaa71a3dd28f36770c857</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_UNSUPPORTED_WRITE</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a6ca87eab23fcc0545414ea5320939428</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_EXT_LEN</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a9a52349b3d3803403883f4f5a524a2d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>QT4</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a7d222d33e2cb4a62c7aa27d6ab79dc4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>qimage_set_save_format</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>af39b1095040dc4809906f47144819a67</anchor>
      <arglist>(const char *ext, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_qimage_error</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a3fc18046b8296217e187a2618c4d52e3</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_qimage_get_savers</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a243086773c43179eaedf77a0fd080fe2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_qimage_identify</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a36209e8f5b4d3b9a75e2418668743289</anchor>
      <arglist>(const char *ptr, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_qimage_load</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>aab2c8881d67468d24ee59f5bdca6565f</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_qimage_save</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a57fbef8eed42488f6d96d3841700777e</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *filetypeext)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>qimageerror</name>
      <anchorfile>simage__qimage_8cpp.html</anchorfile>
      <anchor>a3f09c58839fc5ac815285113445b4802</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_quicktime.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__quicktime_8c.html</filename>
    <includes id="simage__quicktime_8h" name="simage_quicktime.h" local="no" import="no" module="no" objc="no">simage_quicktime.h</includes>
    <class kind="struct">BitmapInfo</class>
    <member kind="define">
      <type>#define</type>
      <name>ERR_BAD_DEPTH</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>ad46e5c61a4c4714cf201658cb8b60482</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_CG</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>ac2a5bc5fc854981db5205ab49ef3dcc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_UNSUPPORTED</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>ae0a294beec77352a523900c0e0a51fc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_WRITE</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>abdfbdd5affe2e04429a4373950c83cd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>argb_to_rgba</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a9295035138d4da77c8483995966a9ff9</anchor>
      <arglist>(uint32_t *px, int width, int height)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char *</type>
      <name>cc_basename</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>aa8f6bf7a446ec5459ac8d26a85c8e3b8</anchor>
      <arglist>(const char *path)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char *</type>
      <name>cc_dirname</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>ae9550f84df72361f6500c80261dbe90f</anchor>
      <arglist>(const char *path)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cfstring_append_ostype</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a205e2222d2c8c86f66871c39254d51d5</anchor>
      <arglist>(CFMutableStringRef str, OSType *t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>create_file</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a1cbad7858ab7de762ee658d4d374e9a1</anchor>
      <arglist>(const char *filename, FSSpec *fss)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>get_importer</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>af55d26d22fab0d4d27f1bf58bcf21ce6</anchor>
      <arglist>(const char *filename, GraphicsImportComponent *c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>open_exporter</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a8b1d87dc628a9d11cbe4e806422cf782</anchor>
      <arglist>(const char *fext, GraphicsExportComponent *ge)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>rgba_to_argb</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>aa9351b7c51d7d85e9566070b90a3eddb</anchor>
      <arglist>(uint32_t *px, int width, int height)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_quicktime_error</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>ae69f00d4adddceedacc1ad7efef7a109</anchor>
      <arglist>(char *cstr, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>simage_quicktime_get_savers</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>abe6f147dc9791880f5c2776092083455</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_quicktime_identify</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a130dcc127dbd1c49ab940c82f18feccd</anchor>
      <arglist>(const char *file, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_quicktime_load</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a0508ebcf6efe8f7335ccf8021a5e3b71</anchor>
      <arglist>(const char *file, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_quicktime_save</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>af85abced60cfb3517e607a9448d9a679</anchor>
      <arglist>(const char *filename, const unsigned char *px, int width, int height, int numcomponents, const char *filetypeext)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>system_is_bigendian</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a1637060c974fada62a44b2cc9baeadd3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>v_flip</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a5543ee9495570ec2fb3be4d4e02e2a6c</anchor>
      <arglist>(const unsigned char *px, int width, int height, int numcomponents, unsigned char *newpx)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>quicktimeerror</name>
      <anchorfile>simage__quicktime_8c.html</anchorfile>
      <anchor>a20b2f15ec5583f8e6bca99d1171d2253</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_rgb.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__rgb_8c.html</filename>
    <includes id="simage__rgb_8h" name="simage_rgb.h" local="no" import="no" module="no" objc="no">simage_rgb.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <class kind="struct">simage_rgb_opendata</class>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>ae162e35f35c095230d397bd9954bacd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_READ</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a6b84d62f127a9e48e8a98f7c4f50f3ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_SIZEZ</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a4a65ea63afd20a0b44f237d4fb4b74b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>read_int</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a295ccde1f86879f48275bbf1ba37b410</anchor>
      <arglist>(FILE *in, int *dst, int n, int swap)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>read_rgb_row_component</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a748471217877115403972cf34b95ca6b</anchor>
      <arglist>(simage_rgb_opendata *od, int y, int c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>read_short</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a8c26ff1ea70b1c52dd84abf2fd95b6fd</anchor>
      <arglist>(FILE *in, short *dst, int n, int swap)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>read_uint</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a6db073bcfb6d9060380fd04919c261bf</anchor>
      <arglist>(FILE *in, unsigned int *dst, int n, int swap)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>read_ushort</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a948b1f7c2c916cac9b4bf0366d7fb393</anchor>
      <arglist>(FILE *in, unsigned short *dst, int n, int swap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_rgb_close</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a4b7afb05ca43f4f986bcf7b8eb9c72c4</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_error</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a431f2419afa1807b7587f2e7fd3f965a</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_identify</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a30d70b21fd0212c83202f365b0506e4e</anchor>
      <arglist>(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_rgb_load</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a3a1ac4edc6ada9f5830a76c535b46ecb</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_rgb_open</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>ad554e3ab5ffa1e1a226f77bcee6aba70</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_read_line</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a328d67939d1fb14590fedb3c1577faa2</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_rgb_save</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a65460499d07adddd8e60ac32f47c3b8c</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int comp)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>write_short</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a3164f77e9771333d4ce6703685213185</anchor>
      <arglist>(FILE *fp, unsigned short val)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>rgberror</name>
      <anchorfile>simage__rgb_8c.html</anchorfile>
      <anchor>a2c150cca9bb604c831691fb7f3ca5994</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_tga.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__tga_8c.html</filename>
    <includes id="simage__tga_8h" name="simage_tga.h" local="no" import="no" module="no" objc="no">simage_tga.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_READ</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a6b84d62f127a9e48e8a98f7c4f50f3ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_UNSUPPORTED</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>ae0a294beec77352a523900c0e0a51fc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>convert_16_to_24</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a9e14d0952777dcb559f9ee9e580f3283</anchor>
      <arglist>(const unsigned char *const src, unsigned char *const dest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>convert_16_to_32</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a347f270aee7fb001519b490a854fa94c</anchor>
      <arglist>(const unsigned char *const src, unsigned char *const dest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>convert_24_to_24</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a8b8578682bc88b8b14a64a5c704615da</anchor>
      <arglist>(const unsigned char *const src, unsigned char *const dest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>convert_32_to_32</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a93074824c7a8c2f8519ae3ee89578c6d</anchor>
      <arglist>(const unsigned char *const src, unsigned char *const dest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>convert_data</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a089ba418bebb6f03205e17106862df1c</anchor>
      <arglist>(const unsigned char *const src, unsigned char *const dest, const int x, const int srcformat, const int destformat)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>getInt16</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a5fea79779d9c63478458efa58e4f3524</anchor>
      <arglist>(unsigned char *ptr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>rle_decode</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a7d85d760052df0d1ca2e0c46ea4fe420</anchor>
      <arglist>(unsigned char *src, unsigned char *dest, const int numbytes, int *rleRemaining, int *rleIsCompressed, unsigned char *rleCurrent, const int rleEntrySize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned char *</type>
      <name>rle_new_packet</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>acf53427290c019fb702d1139ea2ac883</anchor>
      <arglist>(unsigned char *src, int *rleRemaining, int *rleIsCompressed, unsigned char *rleCurrent, const int rleEntrySize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tga_error</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>ac65801300b49e90a1eb4446389be9a71</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tga_identify</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>a34d2e674539332e01a95bd0fb716ce82</anchor>
      <arglist>(const char *filename, const unsigned char *buf, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_tga_load</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>aec52d33f22084c9e89af3020d7fa815c</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>tgaerror</name>
      <anchorfile>simage__tga_8c.html</anchorfile>
      <anchor>ad8e8fbda92dee15b76a3ff42a877de71</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_tiff.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__tiff_8c.html</filename>
    <includes id="simage__tiff_8h" name="simage_tiff.h" local="no" import="no" module="no" objc="no">simage_tiff.h</includes>
    <class kind="struct">simage_tiff_opendata</class>
    <member kind="define">
      <type>#define</type>
      <name>CVT</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ae3ce3f1f3093525937c720ff77ebf1b1</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MEM</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a6ab63185cc5dff3c50c61d99bdf98369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_NO_ERROR</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a2220e71b976471f28149fd35f0d411e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a11304debf1119c31f466e0b96ac65510</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_OPEN_WRITE</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ae162e35f35c095230d397bd9954bacd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_READ</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a6b84d62f127a9e48e8a98f7c4f50f3ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_TIFFLIB</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a54bbb22ea4f81e67fd48f5e13fea572d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_UNSUPPORTED</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ae0a294beec77352a523900c0e0a51fc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_WRITE</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>abdfbdd5affe2e04429a4373950c83cd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pack</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a0163736e617b8a89dc5b706dd3da24a1</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>checkcmap</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a84c0c6a2632130d7f2b05b336aff1090</anchor>
      <arglist>(int n, uint16 *r, uint16 *g, uint16 *b)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>copy_row</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a4a618e633bb2477d38886c05804035f6</anchor>
      <arglist>(unsigned char *ptr, unsigned char *data, int n, int numcomponents)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>interleave_row</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ae31ff13e9e123bc7a6567cfbabc78bc1</anchor>
      <arglist>(unsigned char *ptr, unsigned char *red, unsigned char *blue, unsigned char *green, unsigned char *alpha, int n)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>invert_row</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ac777a388d7f6fafc28ccbf66a11af835</anchor>
      <arglist>(unsigned char *ptr, unsigned char *data, int n, int invert)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>remap_row</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a54c5450d2c7a6e1850ae6fe83ea500d1</anchor>
      <arglist>(unsigned char *ptr, unsigned char *data, int n, unsigned short *rmap, unsigned short *gmap, unsigned short *bmap, unsigned char *amap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_tiff_close</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a5a82b332de3f2df19266498ddcd28675</anchor>
      <arglist>(void *opendata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_error</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a89b41ba5adf0436d7468eedd6ca6fbb3</anchor>
      <arglist>(char *buffer, int buflen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_identify</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ac8f5377f687ceb9a970d75440f97c145</anchor>
      <arglist>(const char *ptr, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>simage_tiff_load</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ab7c958cf5402094bf4a700ab3bcdb5a7</anchor>
      <arglist>(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_tiff_open</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a70081057f429a638e7e47e23db6156b1</anchor>
      <arglist>(const char *filename, int *width, int *height, int *numcomponents)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_read_line</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ad874a9741dd6133549784ebc4cfbb19e</anchor>
      <arglist>(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_tiff_save</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a86bd686f3a3052a2cf17b9d6ad2fa0b9</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>tiff_error</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a0035dee06fdbe80a15df5e5220538e4b</anchor>
      <arglist>(const char *module, const char *fmt, va_list list)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>tiff_try_read_rgba</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a6029e2cfdfca9004a7800dabe2aa9586</anchor>
      <arglist>(TIFF *in, int w, int h, int format, unsigned char *buffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>tiff_warn</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>ab8b7f5b01ed1f0a27dee46c6c0d6dad2</anchor>
      <arglist>(const char *module, const char *fmt, va_list list)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>tifferror</name>
      <anchorfile>simage__tiff_8c.html</anchorfile>
      <anchor>a3f2e6ccb9451820e29cb6b09fc4771e2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_write.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__write_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__jpeg_8h" name="simage_jpeg.h" local="no" import="no" module="no" objc="no">simage_jpeg.h</includes>
    <includes id="simage__png_8h" name="simage_png.h" local="no" import="no" module="no" objc="no">simage_png.h</includes>
    <includes id="simage__tiff_8h" name="simage_tiff.h" local="no" import="no" module="no" objc="no">simage_tiff.h</includes>
    <includes id="simage__rgb_8h" name="simage_rgb.h" local="no" import="no" module="no" objc="no">simage_rgb.h</includes>
    <includes id="simage__gif_8h" name="simage_gif.h" local="no" import="no" module="no" objc="no">simage_gif.h</includes>
    <includes id="simage__eps_8h" name="simage_eps.h" local="no" import="no" module="no" objc="no">simage_eps.h</includes>
    <class kind="struct">_saver_data</class>
    <member kind="define">
      <type>#define</type>
      <name>SIMAGE_ERROR_BUFSIZE</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a9fe9523ba3a0f96d9c8fe8789e0b225e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct _saver_data</type>
      <name>saver_data</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a9c38f0df09df0826192915be3a04ccdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_internal_savers</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a6a00ed3ba9c0065485daef36e573fd38</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>add_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>ade212fbb269b267339e6088516fdf86c</anchor>
      <arglist>(saver_data *saver, int(*save_func)(const char *, const unsigned char *, int, int, int), int(*error_func)(char *, int), const char *extensions, const char *fullname, const char *description, int is_internal, int addbefore)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_saver_data</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a209bb11325af58632b8f2f5f4a8eefb0</anchor>
      <arglist>(saver_data *saver, int(*error_func)(char *, int), const char *extensions, const char *fullname, const char *description, int is_internal, int addbefore)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>add_saver_ext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>aeefeb8eea1e7442d7f9a570a62f10e1b</anchor>
      <arglist>(saver_data *saver, int(*save_func)(const char *, const unsigned char *, int, int, int, const char *), int(*error_func)(char *, int), const char *extensions, const char *fullname, const char *description, int is_internal, int addbefore)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static saver_data *</type>
      <name>find_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a4b3c1dac586b5c2f19d18c664c42716e</anchor>
      <arglist>(const char *filenameextension)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char *</type>
      <name>safe_strdup</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a515d3420e13a8688464ad3b3c89c2ffb</anchor>
      <arglist>(const char *str)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>safe_strfree</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>acfddf08bccdb041eb82db4832bd97342</anchor>
      <arglist>(char *str)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_add_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a427ecb08032d50a7953c8cb5d9bd6388</anchor>
      <arglist>(int(*save_func)(const char *name, const unsigned char *bytes, int width, int height, int nc), int(*error_func)(char *textbuffer, int bufferlen), const char *extensions, const char *fullname, const char *description, int addbefore)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_check_save_supported</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a364baad234011c5f9b4d6af8dded0861</anchor>
      <arglist>(const char *filenameextension)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_get_num_savers</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a589cd048fba3039cb0df3ddf87fdef5d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_saver_description</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a63ea96a19be9763acee4c5d6138d6b78</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_saver_extensions</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a4fd40a1f66bef8c4b3bbb941f29fcfbe</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>simage_get_saver_fullname</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>aa2de3b638b4c773c35ab486092452af7</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>simage_get_saver_handle</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a1a40ff17227c793469a6de7c22c14214</anchor>
      <arglist>(int idx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>simage_remove_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a4072566fefb5c7a34c5d8fcb00f66392</anchor>
      <arglist>(void *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>simage_save_image</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a74a7dbdf0a45f6da8bba17166d6678f5</anchor>
      <arglist>(const char *filename, const unsigned char *bytes, int width, int height, int numcomponents, const char *filenameextension)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>simage_strcasecmp</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>aa0625bd3e6c3f4eacfe4cc30cc853639</anchor>
      <arglist>(const char *str1, const char *str2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>str_tolower</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>afd3e302de899c8e49c3defa428656a98</anchor>
      <arglist>(char *str)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data</type>
      <name>eps_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>ac72faeed3ab07509e1eeacfcfb3f6a72</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>epsext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a434272a855bf95720a1879e86d6d8e0f</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>epsfull</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a45e31e8ccebe412326655030acde1256</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data *</type>
      <name>first_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>af4e870a755b5815b4cb5b8925f8fed25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data</type>
      <name>gif_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a6b27eed5087d0f56a4cdae01727c5b44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>gifext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a050106665ab9c65d95e56dc46b38a31a</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>giffull</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a854588cf9ea0b0894f58c1321a7614f2</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data</type>
      <name>jpeg_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a975201442a8195231b7286d5b30df08e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>jpegext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a24883e5fc8f76d6bd74fe2cb5940de6d</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>jpegfull</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>ac7988c2d30bbb46f13dc27d32c559a70</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data *</type>
      <name>last_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a7e17c5587e25cca8c3ac9ce24e72c2a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data</type>
      <name>png_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>aa4e4676b1d5cbc253fc93c2c96afc043</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>pngext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a5e8625f25118f779a9d524aabae8a904</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>pngfull</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a4087bde3a4c6127f64dbf4ddf7fba2fa</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data</type>
      <name>rgb_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a8921c3828f07d9fe114d0f8c6f8db1bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>rgbext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>aa1d8390e6ccd3cb2febf072950e40230</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>rgbfull</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a7418996fefcbd1f09d751ac19e4a1636</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>simage_error_msg</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a11185af7f5e61ceddc76939da47c9eb5</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static saver_data</type>
      <name>tiff_saver</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a8a3265860d341af567ba754f9edcebde</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>tiffext</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a3c8872e66690a32dc2f1f2754c055fa1</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>tifffull</name>
      <anchorfile>simage__write_8c.html</anchorfile>
      <anchor>a919fb396fb339f25bbaa6e2d9e373510</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>simage_xwd.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>simage__xwd_8c.html</filename>
  </compound>
  <compound kind="file">
    <name>stream.c</name>
    <path>/home/runner/work/simage/simage/src/</path>
    <filename>stream_8c.html</filename>
    <includes id="simage_8h" name="simage.h" local="no" import="no" module="no" objc="no">simage.h</includes>
    <includes id="simage__private_8h" name="simage_private.h" local="no" import="no" module="no" objc="no">simage_private.h</includes>
    <includes id="simage__oggvorbis_8h" name="simage_oggvorbis.h" local="yes" import="no" module="no" objc="no">simage_oggvorbis.h</includes>
    <includes id="simage__libsndfile_8h" name="simage_libsndfile.h" local="yes" import="no" module="no" objc="no">simage_libsndfile.h</includes>
    <class kind="struct">simage_stream_exporter</class>
    <class kind="struct">simage_stream_importer</class>
    <class kind="struct">simage_stream_s</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_internal_exporters</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ad72ad815311e51e7c9101f1728241516</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add_internal_importers</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ae3cb0640bb0eab39e33041e12e857ac8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_close</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a5b10cb26480c72bd93e7abcf9ebfd599</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>s_stream_context_get</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a91fc88fe1e7b5397bbf11983a0f64af7</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_context_set</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ad9cafbd750ceb9b564a7afb9ddfdba06</anchor>
      <arglist>(s_stream *stream, void *context)</arglist>
    </member>
    <member kind="function">
      <type>s_stream *</type>
      <name>s_stream_create</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ac085060e2a689e556a0158fbbf9aaca9</anchor>
      <arglist>(const char *filename, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_destroy</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a8da1765eab30549fdf48f70b5578f469</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_exporter_add</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a200f0531dd434d1c92f5476a4726a1bc</anchor>
      <arglist>(s_stream_create_func *create, s_stream_put_func *put, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_exporter_add_ex</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ab36f2344a823d1bdae7e73376192c75e</anchor>
      <arglist>(s_stream_create_func *create, s_stream_put_func *put, s_stream_seek_func *seek, s_stream_tell_func *tell, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>s_stream_get_buffer</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ae6d43b3b38b4abbb1073bf0ebcd5670e</anchor>
      <arglist>(s_stream *stream, void *buffer, int *size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_importer_add</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a310d04e7012e18033d2a3e51f0eb2dc1</anchor>
      <arglist>(s_stream_open_func *open, s_stream_get_func *get, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>s_stream_importer_add_ex</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ad57d3643ce92f348f0d1588b9280b626</anchor>
      <arglist>(s_stream_open_func *open, s_stream_get_func *get, s_stream_seek_func *seek, s_stream_tell_func *tell, s_stream_close_func *close)</arglist>
    </member>
    <member kind="function">
      <type>s_stream *</type>
      <name>s_stream_open</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ae814cc9ea5f238fff4dcfebe04cda99d</anchor>
      <arglist>(const char *filename, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>s_params *</type>
      <name>s_stream_params</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a1026c2ff2bb109f6b41e31785e73c255</anchor>
      <arglist>(s_stream *stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_stream_put_buffer</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a87a02f510adb825ba66bf42c5f704843</anchor>
      <arglist>(s_stream *stream, void *buffer, int size, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_stream_seek</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a8324ab16a7d9675b41a145154c815a16</anchor>
      <arglist>(s_stream *stream, int offset, int whence, s_params *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>s_stream_tell</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>ad24bb8cb2494052a24c7bf738528c151</anchor>
      <arglist>(s_stream *stream, s_params *params)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static struct simage_stream_exporter *</type>
      <name>exporters</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a7be56cc41d205bf14459a782eb114728</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static struct simage_stream_importer *</type>
      <name>importers</name>
      <anchorfile>stream_8c.html</anchorfile>
      <anchor>a715d387b807932cc16e5ab578d1b8afd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>_loader_data</name>
    <filename>struct__loader__data.html</filename>
    <member kind="variable">
      <type>struct simage_plugin</type>
      <name>funcs</name>
      <anchorfile>struct__loader__data.html</anchorfile>
      <anchor>a75c8e5be9050c0344203ddb93fbee493</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>is_internal</name>
      <anchorfile>struct__loader__data.html</anchorfile>
      <anchor>ae4deb391259c82fdbdff877ac44276d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct _loader_data *</type>
      <name>next</name>
      <anchorfile>struct__loader__data.html</anchorfile>
      <anchor>a55a4995901f1a1d6af2493480c959586</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_open_funcs</type>
      <name>openfuncs</name>
      <anchorfile>struct__loader__data.html</anchorfile>
      <anchor>a968f3af4c476c2cf1fb567d62dbd78c8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>_saver_data</name>
    <filename>struct__saver__data.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>description</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>a06b43f292ea07f3d9f080e7df66f613b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>error_func</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>ae9929bc28e837ffc0360decb6c74c312</anchor>
      <arglist>)(char *textbuffer, int bufferlen)</arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>extensions</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>a239b3d90b6ef0a67a5aa0b7658e32f55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>fullname</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>a106b1ccbacad152756bfc826369d4931</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>is_internal</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>ad5179adbaf2a18c5f81353d142973d91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct _saver_data *</type>
      <name>next</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>a854b0785eb9a3915f5ba3cc9f68d2945</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>save_func</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>a555ecfd6670062c3e5695ea8996c7824</anchor>
      <arglist>)(const char *name, const unsigned char *bytes, int width, int height, int numcomponents)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>save_func_ext</name>
      <anchorfile>struct__saver__data.html</anchorfile>
      <anchor>a03635221acc7577894652138f3afb20a</anchor>
      <arglist>)(const char *name, const unsigned char *bytes, int width, int height, int numcomponents, const char *ext)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>BitmapInfo</name>
    <filename>structBitmapInfo.html</filename>
    <member kind="variable">
      <type>size_t</type>
      <name>bitsPerPixel</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>aa981319c9bd7887124231f1592a73223</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>bytesPerRow</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>a753930206d1e3df6cf40a534f4ce1002</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>data</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>a634c01be892849d0071f5894437f76b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>height</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>ae786d17f05751626b2453ae192b20372</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>numcomponents</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>a3c49637dcc9a929c8cc664511115fd3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>size</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>adc225bc5a17fe1146a2bd5db5dd46641</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>width</name>
      <anchorfile>structBitmapInfo.html</anchorfile>
      <anchor>a757de19cb33425508d39ee28f4f8e2e0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CLIST</name>
    <filename>structCLIST.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>n</name>
      <anchorfile>structCLIST.html</anchorfile>
      <anchor>a45dc4be27394bc1e5b46fb8e1fee798c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CONTRIB *</type>
      <name>p</name>
      <anchorfile>structCLIST.html</anchorfile>
      <anchor>a2b8eedef16f5e74fdf5770dd86a66d40</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CONTRIB</name>
    <filename>structCONTRIB.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>pixel</name>
      <anchorfile>structCONTRIB.html</anchorfile>
      <anchor>ad0ca1763c6f64f297ac6f0af2882475c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>weight</name>
      <anchorfile>structCONTRIB.html</anchorfile>
      <anchor>af210c28f5a6efbaa00f8c4e7f84eb030</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Image</name>
    <filename>structImage.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>bpp</name>
      <anchorfile>structImage.html</anchorfile>
      <anchor>a3286e302b39f4b40c2a80c3b6deb2223</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>data</name>
      <anchorfile>structImage.html</anchorfile>
      <anchor>a22b726c8306f78b5022e0670ddb0937f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>span</name>
      <anchorfile>structImage.html</anchorfile>
      <anchor>a4f59395eb267a4f532a9add4517d5613</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>xsize</name>
      <anchorfile>structImage.html</anchorfile>
      <anchor>a141904daee393ba7cb479f1922cffd4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ysize</name>
      <anchorfile>structImage.html</anchorfile>
      <anchor>a92de8a4790952fc14c88dc6eba0d2116</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>libsndfile_context</name>
    <filename>structlibsndfile__context.html</filename>
    <member kind="variable">
      <type>SNDFILE *</type>
      <name>file</name>
      <anchorfile>structlibsndfile__context.html</anchorfile>
      <anchor>ad97777572cd0ff523af6d37a16f8cddc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SF_INFO</type>
      <name>sfinfo</name>
      <anchorfile>structlibsndfile__context.html</anchorfile>
      <anchor>a1e5c1f42c71abd2b754750a8b25eb8b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double *</type>
      <name>tempbuffer</name>
      <anchorfile>structlibsndfile__context.html</anchorfile>
      <anchor>a9ef152a88b81666d3a69d3a0e0b0f096</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>tempbuffersize</name>
      <anchorfile>structlibsndfile__context.html</anchorfile>
      <anchor>a65bb89d362c9081083f156ab452d4227</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>my_error_mgr</name>
    <filename>structmy__error__mgr.html</filename>
    <member kind="variable">
      <type>struct jpeg_error_mgr</type>
      <name>pub</name>
      <anchorfile>structmy__error__mgr.html</anchorfile>
      <anchor>aac1fb61cc51b4d8edbb44ba85fcccdb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>jmp_buf</type>
      <name>setjmp_buffer</name>
      <anchorfile>structmy__error__mgr.html</anchorfile>
      <anchor>a751138dd0c774fe1d41bb5465b3a7539</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>oggvorbis_reader_context</name>
    <filename>structoggvorbis__reader__context.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>current_section</name>
      <anchorfile>structoggvorbis__reader__context.html</anchorfile>
      <anchor>a4fa6ea1b6a1680959fe4aa3fd9b7ff1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FILE *</type>
      <name>file</name>
      <anchorfile>structoggvorbis__reader__context.html</anchorfile>
      <anchor>a1ad606551586475b343c846571929d61</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OggVorbis_File</type>
      <name>vorbisfile</name>
      <anchorfile>structoggvorbis__reader__context.html</anchorfile>
      <anchor>a5b03efe849f42c48338c147e5de896c1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_image_s</name>
    <filename>structsimage__image__s.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>components</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a009b9b4bf2b95afa705fa0c1fa87c853</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>data</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>ae584d3030905c4de90891548069d76fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>didalloc</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>aa0aeee0d77fc3e3da9ff9d0b56089a28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>height</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a6ada336a4a044c130a7d8a7173c108aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>oktoreadall</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a7b6f8b750b5a0368d1c675665fc58c8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>opendata</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a389e1154f3a95f86fb70d9ebf469dfd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>openfilename</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a02fca0f9d8b8f43d3d9861472111c235</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_open_funcs</type>
      <name>openfuncs</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a368ac7c6f7734bccd6460309b66ad5a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>order</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>ad92433506420b7aa24fbe8821045f7a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>width</name>
      <anchorfile>structsimage__image__s.html</anchorfile>
      <anchor>a47fcc1210e604cf8ce1b2c387e1de802</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_movie_exporter</name>
    <filename>structsimage__movie__exporter.html</filename>
    <member kind="variable">
      <type>s_movie_close_func *</type>
      <name>close</name>
      <anchorfile>structsimage__movie__exporter.html</anchorfile>
      <anchor>a608cb2911928be753abbf075d8019b54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_create_func *</type>
      <name>create</name>
      <anchorfile>structsimage__movie__exporter.html</anchorfile>
      <anchor>acd270e706052afef24f4fc656bd7de87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_movie_exporter *</type>
      <name>next</name>
      <anchorfile>structsimage__movie__exporter.html</anchorfile>
      <anchor>a652ac169cd962c946fd506b41da55d06</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_put_func *</type>
      <name>put</name>
      <anchorfile>structsimage__movie__exporter.html</anchorfile>
      <anchor>a1e40e0090e9c2f8e4befba6fb83fa8dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_movie_importer</name>
    <filename>structsimage__movie__importer.html</filename>
    <member kind="variable">
      <type>s_movie_close_func *</type>
      <name>close</name>
      <anchorfile>structsimage__movie__importer.html</anchorfile>
      <anchor>a5ffbca7780171a2e291d81e8219ddd9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_get_func *</type>
      <name>get</name>
      <anchorfile>structsimage__movie__importer.html</anchorfile>
      <anchor>a1be3e8799db6fca9f2f2642948fe6f5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_movie_importer *</type>
      <name>next</name>
      <anchorfile>structsimage__movie__importer.html</anchorfile>
      <anchor>a3c5242273c469e45a6e97ea03e062a2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_open_func *</type>
      <name>open</name>
      <anchorfile>structsimage__movie__importer.html</anchorfile>
      <anchor>a52e6839da3909b95efac322d7432de54</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_movie_s</name>
    <filename>structsimage__movie__s.html</filename>
    <member kind="variable">
      <type>s_movie_close_func *</type>
      <name>close</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>ace04a87845c7468261656bc758c2ff7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_create_func *</type>
      <name>create</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>a9f62fac3f735a1d92091a4ea34a4c0da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>filename</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>a2e1cf80e0a1ecd6b62f208375c6a967c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_get_func *</type>
      <name>get</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>a27e64947bd99710351507289f83e89e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_open_func *</type>
      <name>open</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>a8144580f60c118cebf933ca9e0c79cd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_params *</type>
      <name>params</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>a76cdb35f4aff1cb922dba46576843e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_movie_put_func *</type>
      <name>put</name>
      <anchorfile>structsimage__movie__s.html</anchorfile>
      <anchor>a3e8fd9ba99329d000d50028229fb02ef</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_open_funcs</name>
    <filename>structsimage__open__funcs.html</filename>
    <member kind="variable">
      <type>void(*</type>
      <name>close_func</name>
      <anchorfile>structsimage__open__funcs.html</anchorfile>
      <anchor>af0cdfefdf4c9762c752d4491c9f1b4b7</anchor>
      <arglist>)(void *opendata)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>next_line_func</name>
      <anchorfile>structsimage__open__funcs.html</anchorfile>
      <anchor>a83da89a1860e4530fa6dbdd5b0c4a7c2</anchor>
      <arglist>)(void *opendata, unsigned char *buf)</arglist>
    </member>
    <member kind="variable">
      <type>void *(*</type>
      <name>open_func</name>
      <anchorfile>structsimage__open__funcs.html</anchorfile>
      <anchor>a2b0d4a64f987c79b04be3a030bb96b96</anchor>
      <arglist>)(const char *filename, int *w, int *h, int *nc)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>read_line_func</name>
      <anchorfile>structsimage__open__funcs.html</anchorfile>
      <anchor>a4bef0586fa3de63e1734b5ef23407016</anchor>
      <arglist>)(void *opendata, int y, unsigned char *buf)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_param_data</name>
    <filename>structsimage__param__data.html</filename>
    <member kind="variable">
      <type>union simage_param_data::@0</type>
      <name>data</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a6815e1e6bf4c947ebd0df1a41ee79665</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>doubledata</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>ad348072509f455a30073730bb3fc1d45</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>floatdata</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a823a925d7530fe1a7921314bc0090609</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void(*</type>
      <name>functiondata</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a9606f220d5c385b066d3aa11ca2e18ff</anchor>
      <arglist>)()</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>integerdata</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a222c6369a5c4fa8e063f48e9ec7e380f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a82f4e9225fc35a600d9b92dc35719a05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_param_data *</type>
      <name>next</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a1ac5775040e9076c847df03fe022ad11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>pointerdata</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a68fef7b995a96bc93b6cc95536135434</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>stringdata</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a66ab649918456e66d00ac465683d37ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>type</name>
      <anchorfile>structsimage__param__data.html</anchorfile>
      <anchor>a0a278b154e07efce1aa356e062317af3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_parameters_s</name>
    <filename>structsimage__parameters__s.html</filename>
    <member kind="variable">
      <type>struct simage_param_data *</type>
      <name>list</name>
      <anchorfile>structsimage__parameters__s.html</anchorfile>
      <anchor>a9ea425a93baa9558982e47c50fb21299</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_plugin</name>
    <filename>structsimage__plugin.html</filename>
    <member kind="variable">
      <type>int(*</type>
      <name>error_func</name>
      <anchorfile>structsimage__plugin.html</anchorfile>
      <anchor>af1df57d0528d65739e4fa591844ff777</anchor>
      <arglist>)(char *textbuffer, int bufferlen)</arglist>
    </member>
    <member kind="variable">
      <type>int(*</type>
      <name>identify_func</name>
      <anchorfile>structsimage__plugin.html</anchorfile>
      <anchor>a75167cf32e01b7c275e4b4817205918d</anchor>
      <arglist>)(const char *filename, const unsigned char *header, int headerlen)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *(*</type>
      <name>load_func</name>
      <anchorfile>structsimage__plugin.html</anchorfile>
      <anchor>a16b93336d2f944e7365d8496b94fe520</anchor>
      <arglist>)(const char *name, int *width, int *height, int *numcomponents)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_rgb_opendata</name>
    <filename>structsimage__rgb__opendata.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>compressed</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>adeed3cf070ee1a1e453955ace31d1d65</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>h</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>aa8b437734a813efcd5b9701025fc604a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FILE *</type>
      <name>in</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>aeffc14d21d16cdbdb7ef3f6d01e6bc53</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>nc</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>ad978a7d053eae11ea01fd73349322125</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>rlebuf</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>a99b7f575793d18fa38a451910a81d669</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>rlebuflen</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>a9a3417d7ac0afba01fd892c38f0c0a85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int *</type>
      <name>rowlen</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>ae92fbec9835be90801850b1ee9d7ffa8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int *</type>
      <name>rowseek</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>a8666e264ef8540eb80a94e43f18db5dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>tmpbuf</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>a36ab0c839b4b9811a901b66835493f0b</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>w</name>
      <anchorfile>structsimage__rgb__opendata.html</anchorfile>
      <anchor>af369bfb43670beac6b1a331111514026</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_stream_exporter</name>
    <filename>structsimage__stream__exporter.html</filename>
    <member kind="variable">
      <type>s_stream_close_func *</type>
      <name>close</name>
      <anchorfile>structsimage__stream__exporter.html</anchorfile>
      <anchor>a85ba10cec0dd7d755bce858f938e2835</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_create_func *</type>
      <name>create</name>
      <anchorfile>structsimage__stream__exporter.html</anchorfile>
      <anchor>aa20a544ec5b0c92922d38b3bacf153f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_stream_exporter *</type>
      <name>next</name>
      <anchorfile>structsimage__stream__exporter.html</anchorfile>
      <anchor>a97dd7e265ce032c29166e53c4a904476</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_put_func *</type>
      <name>put</name>
      <anchorfile>structsimage__stream__exporter.html</anchorfile>
      <anchor>a1d590df145e04152a92e4f1f8b1d5fc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_seek_func *</type>
      <name>seek</name>
      <anchorfile>structsimage__stream__exporter.html</anchorfile>
      <anchor>ac4df94ea27363c88a46f4719d87e671d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_tell_func *</type>
      <name>tell</name>
      <anchorfile>structsimage__stream__exporter.html</anchorfile>
      <anchor>a23465379c27c3a8e77aa5c0002139f80</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_stream_importer</name>
    <filename>structsimage__stream__importer.html</filename>
    <member kind="variable">
      <type>s_stream_close_func *</type>
      <name>close</name>
      <anchorfile>structsimage__stream__importer.html</anchorfile>
      <anchor>a693570224147af8894b375aca87f878a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_get_func *</type>
      <name>get</name>
      <anchorfile>structsimage__stream__importer.html</anchorfile>
      <anchor>ac5eedd5d2ce8f51c48cf7255ffe82127</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct simage_stream_importer *</type>
      <name>next</name>
      <anchorfile>structsimage__stream__importer.html</anchorfile>
      <anchor>a36306d789cab3632854b2dd399cff7fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_open_func *</type>
      <name>open</name>
      <anchorfile>structsimage__stream__importer.html</anchorfile>
      <anchor>ade1eb81c2a4d725ac1bd8a81264e6b5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_seek_func *</type>
      <name>seek</name>
      <anchorfile>structsimage__stream__importer.html</anchorfile>
      <anchor>adb3774a8d2da561aab529f82e338f963</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_tell_func *</type>
      <name>tell</name>
      <anchorfile>structsimage__stream__importer.html</anchorfile>
      <anchor>abf02ebebdf43f92cab0a71305fb1b6e7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_stream_s</name>
    <filename>structsimage__stream__s.html</filename>
    <member kind="variable">
      <type>s_stream_close_func *</type>
      <name>close</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>abd94de85a9a73fb6d67b4619e768710a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>context</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>a2cd2504a802aa0c1abd1e453bd385279</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_create_func *</type>
      <name>create</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>a052b003bbfe3b3117c86f2202daa3311</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>filename</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>a5263f309dd0bc0637c081d4e536b3c16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_get_func *</type>
      <name>get</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>a351fc8d9a077ade059d331f18cf99351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_open_func *</type>
      <name>open</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>a0504cd4f0ee5d8b89a60b1cd6d55493c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_params *</type>
      <name>params</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>a1ab42ee7675f0def4e1335482d88059a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_put_func *</type>
      <name>put</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>af808054153dcb933f7c45835fd1a15f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_seek_func *</type>
      <name>seek</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>af6f08a715dac9ca20a5f48c7015ee163</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>s_stream_tell_func *</type>
      <name>tell</name>
      <anchorfile>structsimage__stream__s.html</anchorfile>
      <anchor>aa10cdf5a3292268e8971624a54c7bd3b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>simage_tiff_opendata</name>
    <filename>structsimage__tiff__opendata.html</filename>
    <member kind="variable">
      <type>uint16</type>
      <name>bitspersample</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a88c62a1ffe3a404e6844cf9269e3f191</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16 *</type>
      <name>blue</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>aaf42388524dc07e22452c327387fa3a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16</type>
      <name>config</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a5e2f9b72c93d51217d37322ba9d5059d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>format</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>ab2b5dd2207bde08e194049f76d486773</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16 *</type>
      <name>green</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>abb2f3c5cb4b6c6ea21013907f786f968</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32</type>
      <name>h</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a14520edaecbc6029bd67b68b668c3bf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TIFF *</type>
      <name>in</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>ab573c9ad7399e439b178b1f333a1fed4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>inbuf</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a0310b30a56a287c6bc2f8fe963d328a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16</type>
      <name>photometric</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>ab4c87e4f5352d541b3909033cf64f62f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16 *</type>
      <name>red</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a6fd1ce4f8718caea4cf9d886c7e3c128</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>rowsize</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a987202cc1c50d8e61a0e22fdffc4c592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16</type>
      <name>samplesperpixel</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a0bbf1bb8749d3d2c5485b782d3edda3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32</type>
      <name>w</name>
      <anchorfile>structsimage__tiff__opendata.html</anchorfile>
      <anchor>a94d2e5a66c838c036f2d49a776ad63c8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>simage</title>
    <filename>index.html</filename>
  </compound>
</tagfile>
