/* GStreamer
 * Copyright (C) 2023 FIXME <hbravo.teresa@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstgzdec
 *
 * The gzdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! gzdec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include "gstgzdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_gzdec_debug_category);
#define GST_CAT_DEFAULT gst_gzdec_debug_category

/* prototypes */


static void gst_gzdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_gzdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
// static void gst_gzdec_dispose (GObject * object);
static void gst_gzdec_finalize (GObject * object);

static GstFlowReturn gst_gzdec_chain (GstPad * pad, GstObject * parent, GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */
// The plugin should only have one sink pad and one source pad.
static GstStaticPadTemplate 
sink_template =
GST_STATIC_PAD_TEMPLATE (
  "sink",                   // Short name for the pad
  GST_PAD_SINK,             // Pad direction
  GST_PAD_ALWAYS,           // Existence property (allways/sometimes/request)
  GST_STATIC_CAPS ("ANY")   // Supported types by this element (capabilities)
),

src_template =
GST_STATIC_PAD_TEMPLATE (
  "source",
  GST_PAD_SRC,
  GST_PAD_ALWAYS,
  GST_STATIC_CAPS ("ANY")
);



/* class initialization */
G_DEFINE_TYPE (GstGzdec, gst_gzdec, GST_TYPE_ELEMENT);
GST_ELEMENT_REGISTER_DEFINE(gzdec, "gzdec", GST_RANK_NONE, GST_TYPE_GZDEC);


// The _class_init() function is used to initialize the class only once
static void
gst_gzdec_class_init (GstGzdecClass * klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
    //   GstAudioSinkClass *audio_sink_class = GST_AUDIO_SINK_CLASS (klass);

    gst_element_class_set_static_metadata (element_class, "Gzip decoder", 
    "Decoder/Uncompressor/gzip", 
    "Uncompresses gzip compressed streams",
    "Teresa Hernando Bravo <hbravo.teresa@gmail.com>");

    /* Setting up pads and setting metadata should be moved to
    base_class_init if you intend to subclass this class. */
    // Registering the Pads
    gst_element_class_add_static_pad_template (element_class, &src_template);
    gst_element_class_add_static_pad_template (element_class, &sink_template);
}

// The _init() function initializes a specific instance of this type
static void
gst_gzdec_init (GstGzdec * gzdec)
{
    /* pad through which data comes in to the element */
    gzdec->sinkpad = gst_pad_new_from_static_template (&sink_template, "sink");
    /* pads are configured here with gst_pad_set_*_function () */

    
    /* configure chain function on the pad before adding
     * the pad to the element */
    // In the chain function all data processing takes place
    gst_pad_set_chain_function (gzdec->sinkpad, GST_DEBUG_FUNCPTR (gst_gzdec_chain));


    gst_element_add_pad (GST_ELEMENT (gzdec), gzdec->sinkpad);


    /* pad through which data goes out of the element */
    gzdec->srcpad = gst_pad_new_from_static_template (&src_template, "src");
    gst_element_add_pad (GST_ELEMENT (gzdec), gzdec->srcpad);

    /* properties initial value */
    gzdec->silent = FALSE;
}


void
gst_gzdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstGzdec *gzdec = GST_GZDEC (object);

  GST_DEBUG_OBJECT (gzdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_gzdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstGzdec *gzdec = GST_GZDEC (object);

  GST_DEBUG_OBJECT (gzdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static GstFlowReturn
gst_gzdec_chain (GstPad *pad, GstObject *parent, GstBuffer *buf)
{
    GstGzdec *gzdec = GST_GZDEC (parent);
    z_stream *strm = gzdec->gzipStrm;

    if (!gzdec->silent)
        g_print ("Have data of size %" G_GSIZE_FORMAT" bytes!\n",
            gst_buffer_get_size (buf));



    return gst_gzedc_inf(&gzdec, &gzdec->gzipStrm, buf);

}


GstFlowReturn
gst_gzedc_inf (GstGzdec *gzdec, z_stream *strm, GstBuffer *bufIn)
{
    int ret;                    // Used for zlib return codes
    unsigned have;              // The amount of data returned from inflate()
    // z_stream strm;              // Used to pass information to and from the zlib routines, and to mantain the inflate() state
    // unsigned char in[CHUNK];    // Input buffer for inflate()
    // unsigned char out[CHUNK];   // Output buffer for inflate()

    GstMapInfo map;
    guint8 *in_data;
    gsize in_size;

    // Output buffer for inflate()
    guint8* out = (guint8 *)g_malloc_n(CHUNK, sizeof(guint8));

    /* allocate inflate state */
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->avail_in = 0;
    strm->next_in = Z_NULL;
    // ret = inflateInit(strm);
    // ret = inflateInit2(strm, -MAX_WBITS);
    if (Z_OK != inflateInit2(strm,32))
        g_print ("inflateInit2 returned !Z_OK \n");
    if (ret != Z_OK)
        return ret;

    gst_buffer_map (bufIn, &map, GST_MAP_READ);

    /* decompress until deflate stream ends or end of file */
    size_t out_size = 0;

    do 
    {
        strm->avail_in = map.size;  // The number of bytes read
        if (strm->avail_in == 0)
            break;
    
        strm->next_in = map.data;  // A pointer to avail_in bytes
        /* run inflate() on input until output buffer not full */
        do
        {
            // Decode the data
            strm->avail_out = CHUNK; 
            strm->next_out = out;    
            ret = inflate(strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                g_print ("Z_NEED_DICT :( \n");
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
                g_print ("Z_DATA_ERROR :( \n");
            case Z_MEM_ERROR:
                (void)inflateEnd(strm);
                g_print ("Z_MEM_ERROR :( \n");
                return ret;
            }
            have = CHUNK - strm->avail_out;

            out_size += have;

            if (ret != Z_OK){
                 g_print ("ret not Z_OK :( \n");
                return ret;
            }
        
        } while (strm->avail_out == 0);
        assert(strm->avail_in == 0); /* all input will be used */
        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
    assert(ret == Z_STREAM_END); /* stream will be complete */

    /* clean up and return */
    gst_buffer_unmap(bufIn, &map);
    (void)inflateEnd(strm);
    // return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;

    return gst_pad_push (gzdec->srcpad, out);

}


/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif