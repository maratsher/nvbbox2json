# Example pipelines

```
gst-launch-1.0 -e tcpserversrc host=127.0.0.1 port=5000 ! videoparse format=rgba width=1920 height=1080 framerate=30/1 ! nvvideoconvert ! m.sink_0 nvstreammux name=m batch-size=1 width=1920 height=1080 ! nvinfer config-file-path=/opt/nvidia/deepstream/deepstream-6.3/sources/objectDetector_Yolo/config_infer_primary_yoloV3.txt ! nvtracker ll-lib-file=/opt/nvidia/deepstream/deepstream-6.3/lib/libnvds_nvmultiobjecttracker.so tracker-width=1080 tracker-height=1920  ! nvvideoconvert ! nvdsosd ! tee name=t ! queue ! nveglglessink sync=false t. ! queue ! nvobjmeta2json ! tcpclientsink host=127.0.0.1 port=5001
```