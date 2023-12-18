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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_GZDEC_H_
#define _GST_GZDEC_H_

#include <zlib.h>
#include <assert.h>

G_BEGIN_DECLS

/* Standard macros for defining types for this element.  */
#define GST_TYPE_GZDEC   (gst_gzdec_get_type())
#define GST_GZDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_GZDEC,GstGzdec))
#define GST_GZDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_GZDEC,GstGzdecClass))
#define GST_IS_GZDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_GZDEC))
#define GST_IS_GZDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_GZDEC))

#define CHUNK 128000 //Size of the output buffer when decoding the stream

typedef struct _GstGzdec GstGzdec;
typedef struct _GstGzdecClass GstGzdecClass;

/* Definition of structure storing data for this element. */
struct _GstGzdec
{  
  GstElement element;

  GstPad *sinkpad, *srcpad;

  z_streamp gzipStrm;

  gboolean silent;
};

/* Standard definition defining a class for this element. */
struct _GstGzdecClass
{
  GstElementClass parent_class;
};

/* Standard function returning type information. */
GType gst_gzdec_get_type (void);

G_END_DECLS

#endif