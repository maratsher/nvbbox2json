/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
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

#ifndef _GST_NVOBJMETA2JSON_H_
#define _GST_NVOBJMETA2JSON_H_

#include <gst/base/gstbasetransform.h>

G_BEGIN_DECLS

#define GST_TYPE_NVOBJMETA2JSON   (gst_nvobjmeta2json_get_type())
#define GST_NVOBJMETA2JSON(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_NVOBJMETA2JSON,GstNvobjmeta2json))
#define GST_NVOBJMETA2JSON_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_NVOBJMETA2JSON,GstNvobjmeta2jsonClass))
#define GST_IS_NVOBJMETA2JSON(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_NVOBJMETA2JSON))
#define GST_IS_NVOBJMETA2JSON_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_NVOBJMETA2JSON))

typedef struct _GstNvobjmeta2json GstNvobjmeta2json;
typedef struct _GstNvobjmeta2jsonClass GstNvobjmeta2jsonClass;

struct _GstNvobjmeta2json
{
  GstBaseTransform base_nvobjmeta2json;

};

struct _GstNvobjmeta2jsonClass
{
  GstBaseTransformClass base_nvobjmeta2json_class;
};

GType gst_nvobjmeta2json_get_type (void);

G_END_DECLS

#endif
