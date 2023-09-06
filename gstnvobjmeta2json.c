/* 
    created by maratsher 2023
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <glib.h>
#include <gst/base/gstbasetransform.h>
#include "gstnvobjmeta2json.h"
#include "gstnvdsmeta.h"
#include <json-c/json.h>
#include <json-c/json_object.h>

GST_DEBUG_CATEGORY_STATIC (gst_nvobjmeta2json_debug_category);
#define GST_CAT_DEFAULT gst_nvobjmeta2json_debug_category

/* prototypes */
static GstFlowReturn gst_nvobjmeta2json_prepare_output_buffer (GstBaseTransform *
    trans, GstBuffer * input, GstBuffer ** outbuf);
static GstFlowReturn gst_nvobjmeta2json_transform (GstBaseTransform * trans,
    GstBuffer * inbuf, GstBuffer * outbuf);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_nvobjmeta2json_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    //GST_STATIC_CAPS ("application/unknown")
    GST_STATIC_CAPS_ANY
    );

static GstStaticPadTemplate gst_nvobjmeta2json_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    //GST_STATIC_CAPS ("application/unknown")
    GST_STATIC_CAPS_ANY
    //GST_STATIC_CAPS ("text/plain; charset=UTF-8")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstNvobjmeta2json, gst_nvobjmeta2json, GST_TYPE_BASE_TRANSFORM,
  GST_DEBUG_CATEGORY_INIT (gst_nvobjmeta2json_debug_category, "nvobjmeta2json", 0,
  "debug category for nvobjmeta2json element"));

static void
gst_nvobjmeta2json_class_init (GstNvobjmeta2jsonClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_nvobjmeta2json_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_nvobjmeta2json_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  base_transform_class->prepare_output_buffer = GST_DEBUG_FUNCPTR (gst_nvobjmeta2json_prepare_output_buffer);
  base_transform_class->transform = GST_DEBUG_FUNCPTR (gst_nvobjmeta2json_transform);

}

static void
gst_nvobjmeta2json_init (GstNvobjmeta2json *nvobjmeta2json)
{
}

static GstFlowReturn
gst_nvobjmeta2json_prepare_output_buffer (GstBaseTransform * trans, GstBuffer * input,
    GstBuffer ** outbuf)
{
    GstNvobjmeta2json *nvobjmeta2json = GST_NVOBJMETA2JSON (trans);
    GST_DEBUG_OBJECT (nvobjmeta2json, "prepare_output_buffer");

    NvDsBatchMeta *batch_meta = NULL;
    NvDsObjectMeta *obj_meta = NULL;
    GList *l;
    GList *g;

    // Получаем метаданные пакета из буфера
    batch_meta = gst_buffer_get_nvds_batch_meta (input);
    if (batch_meta == NULL) {
        return GST_FLOW_ERROR;
    }

    // Создание корневого объекта JSON
    struct json_object *json_root = json_object_new_object();

    // Добавление свойств в корневой объект
    json_object_object_add(json_root, "function", json_object_new_string("neyron"));
    json_object_object_add(json_root, "channel", json_object_new_int(1));
    json_object_object_add(json_root, "frame", json_object_new_int(0));

    // Создание массива json объектов
    struct json_object *objects_array = json_object_new_array();


    // Итерируемся по объектам в пакете
    for (l = batch_meta->frame_meta_list; l != NULL; l = l->next) {
        NvDsFrameMeta *frame_meta = (NvDsFrameMeta *) (l->data);

        for (g = frame_meta->obj_meta_list; g != NULL; g = g->next) {

            NvDsObjectMeta *obj_meta = (NvDsObjectMeta *) (g->data);
            // Извлекаем данные из NvDsObjectMeta
            gint obj_id = obj_meta->unique_component_id;
            gfloat confidence = obj_meta->confidence;
            gint left = obj_meta->rect_params.left;
            gint top = obj_meta->rect_params.top;
            gint width = obj_meta->rect_params.width;
            gint height = obj_meta->rect_params.height;
            gchar* obj_class = obj_meta->obj_label;

            // Создаем JSON объект для текущего объекта
            json_object *json_obj = json_object_new_object();
            json_object_object_add(json_obj, "clazz", json_object_new_string((const char *)obj_class));
            json_object_object_add(json_obj, "id", json_object_new_int(0));
            json_object_object_add(json_obj, "lostCount", json_object_new_int(0));
            json_object_object_add(json_obj, "x", json_object_new_int((int)left));
            json_object_object_add(json_obj, "y", json_object_new_int((int)top));
            json_object_object_add(json_obj, "width", json_object_new_int((int)width));
            json_object_object_add(json_obj, "height", json_object_new_int((int)height));
            json_object_object_add(json_obj, "confidence", json_object_new_double((double)confidence));

            // Добавляем JSON объект в список
            json_object_array_add(objects_array, json_obj);
        }
    }

    // Добавляем JSON объект в корневой JSON
    json_object_object_add(json_root, "objects", objects_array);

    // Конвертируем JSON в строку
    const gchar *json_str = json_object_to_json_string_ext(json_root, JSON_C_TO_STRING_SPACED);
    const gchar *json_str_copy = g_strdup(json_str);
    gsize json_str_copy_len = strlen(json_str_copy);

    // Задаем нужный размер выходному буфферу
    *outbuf = gst_buffer_new_allocate (NULL, json_str_copy_len, 0);

    //Получаем доступ к данным
    GstMapInfo map;
    if (!gst_buffer_map(*outbuf, &map, GST_MAP_WRITE)) {
        g_printerr("Failed to map output buffer\n");
        return GST_FLOW_ERROR;
    }

    // Копируем данные из json_str_copy в буфер
    memcpy(map.data, json_str_copy, json_str_copy_len);

    //Освобождение ресурсов
    gst_buffer_unmap(*outbuf, &map);
    g_free(json_str_copy);
    json_object_put(json_root);

    // Отправляем буфер дальше по пайплайну
    return GST_FLOW_OK;
}

/* transform */
static GstFlowReturn
gst_nvobjmeta2json_transform (GstBaseTransform * trans, GstBuffer * inbuf,
    GstBuffer * outbuf)
{
    GstNvobjmeta2json *nvobjmeta2json = GST_NVOBJMETA2JSON (trans);
    GST_DEBUG_OBJECT (nvobjmeta2json, "transform");

    return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "nvobjmeta2json", GST_RANK_NONE,
      GST_TYPE_NVOBJMETA2JSON);
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

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    nvobjmeta2json,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

