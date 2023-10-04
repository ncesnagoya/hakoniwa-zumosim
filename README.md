# hakoniwa-zumosim

## 前提とする環境

* OS
  * Windows 10/11 WSL2 Ubuntu 22.04.1 LTS
* Docker
  * WLS2/Ubuntu 上で [Docker Engine](https://docs.docker.com/engine/install/ubuntu/) がインストールされていること

## 事前準備

以下のコマンドを実行して、docker サービスを起動してください。

```
sudo chown <owner> /var/run/docker.sock
```

`<owner>` は、当該コマンド実行ユーザ名です。

```
sudo service docker start
```

Unityアプリケーション(ZumoApp.zip)を以下からダウンロードしてください。

https://github.com/ncesnagoya/hakoniwa-unity-zumo/releases/tag/v1.1.0

ダウンロードしたファイルを解凍し、`hakoniwa-zumosim`直下に配置してください。

![image](https://github.com/ncesnagoya/hakoniwa-zumosim/assets/164193/b85fee35-0af8-4fdd-b5f0-bdeca3d378b4)

成功するとこうなります。

```
$ ls ZumoApp
core_config.json      LoginRobot.json             pdu_readers.json       shm_methods.json
custom.json           model_Data                  pdu_writers.json       UnityCrashHandler64.exe
hakoniwa_core.log     model.exe                   reader_connector.json  UnityPlayer.dll
hakoniwa_path.json    MonoBleedingEdge            RosTopics.json         writer_connector.json
HakoniwaSimTime.json  pdu_channel_connector.json  ros_types
inside_assets.json    pdu_configs.json            rpc_methods.json
```

## ビルド方法

アプリケーションは以下の手順でビルドします。

```
% bash docker/build.bash <aplname>
```

`<aplname>` は、ビルド対象とするアプリケーション名です。

TODO: <aplname> に対応するアプリケーションをビルド環境にどうやって取り込むか要検討

## 箱庭起動方法（本番用）

以下のコマンドを実行することで、箱庭を起動できます。

補足：アプリケーションのシミュレーション実行開始のトリガは、Unity側で行います。

```
% bash simstart.bash 
```

成功すると端末に以下のログが出力されます。

```
$ bash simstart.bash 
INFO: Simulatin start
x86_64
wsl2
INFO: ACTIVATING HAKO-CONDUCTOR
OPEN RECIEVER UDP PORT=172.25.195.216:54001
OPEN SENDER UDP PORT=172.25.195.216:54002
delta_msec = 20
max_delay_msec = 1
Server Start: 172.25.195.216:50051
[UnityMemory] Configuration Parameters - Can be set up in boot.config
    "memorysetup-bucket-allocator-granularity=16"
    "memorysetup-bucket-allocator-bucket-count=8"
    "memorysetup-bucket-allocator-block-size=4194304"
    "memorysetup-bucket-allocator-block-count=1"
    "memorysetup-main-allocator-block-size=16777216"
    "memorysetup-thread-allocator-block-size=16777216"
    "memorysetup-gfx-main-allocator-block-size=16777216"
    "memorysetup-gfx-thread-allocator-block-size=16777216"
    "memorysetup-cache-allocator-block-size=4194304"
    "memorysetup-typetree-allocator-block-size=2097152"
    "memorysetup-profiler-bucket-allocator-granularity=16"
    "memorysetup-profiler-bucket-allocator-bucket-count=8"
    "memorysetup-profiler-bucket-allocator-block-size=4194304"
    "memorysetup-profiler-bucket-allocator-block-count=1"
    "memorysetup-profiler-allocator-block-size=16777216"
    "memorysetup-profiler-editor-allocator-block-size=1048576"
    "memorysetup-temp-allocator-size-main=4194304"
    "memorysetup-job-temp-allocator-block-size=2097152"
    "memorysetup-job-temp-allocator-block-size-background=1048576"
    "memorysetup-job-temp-allocator-reduction-small-platforms=262144"
    "memorysetup-allocator-temp-initial-block-size-main=262144"
    "memorysetup-allocator-temp-initial-block-size-worker=262144"
    "memorysetup-temp-allocator-size-background-worker=32768"
    "memorysetup-temp-allocator-size-job-worker=262144"
    "memorysetup-temp-allocator-size-preload-manager=262144"
    "memorysetup-temp-allocator-size-nav-mesh-worker=65536"
    "memorysetup-temp-allocator-size-audio-worker=65536"
    "memorysetup-temp-allocator-size-cloud-worker=32768"
    "memorysetup-temp-allocator-size-gfx=262144"
INFO: ACTIVATING ZUMOSIM-RUNNER
asset_name = zumosim-runner
config path = ./runtime/custom.json
delta_time_msec = 20
INFO: shmget() key=255 size=1129352 
Robot: ZumoRoboModel, PduWriter: ZumoRoboModel_actuator
channel_id: 1 pdu_size: 24
INFO: ZumoRoboModel create_lchannel: logical_id=1 real_id=0 size=24
WAIT START
Press ENTER to stop...
subscribe_pdu_channel: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: SubscribePduChannelRequest { asset_name: "UnityAsset", channel_id: 1, pdu_size: 24, listen_udp_ip_port: "172.25.192.1:54003", method_type: "UDP", robo_name: "ZumoRoboModel" }, extensions: Extensions }
create_asset_sub_pdu
create_pdu_channel: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: CreatePduChannelRequest { asset_name: "UnityAsset", channel_id: 0, pdu_size: 104, method_type: "UDP", robo_name: "ZumoRoboModel" }, extensions: Extensions }
INFO: shmget() key=255 size=1129352 
INFO: ZumoRoboModel create_lchannel: logical_id=0 real_id=1 size=104
create_asset_pub_pdu: robo_name=ZumoRoboModel channel_id=0 real_id=1
create_asset_pub_pdu: channel_ID=0
register: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: AssetInfo { name: "UnityAsset" }, extensions: Extensions }
asset_notification_start: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: AssetInfo { name: "UnityAsset" }, extensions: Extensions }
```

また、

## 箱庭起動とシミュレーション実行方法（デバッグ用）

### 箱庭起動方法

以下のコマンドを実行することで、箱庭を起動できます。

補足：アプリケーションのシミュレーション実行開始のトリガは、Unity側で行います。

```
% bash docker/run.bash 
```

成功するとこなります。

```
$ bash simstart.bash 
INFO: Simulatin start
x86_64
wsl2
INFO: ACTIVATING HAKO-CONDUCTOR
OPEN RECIEVER UDP PORT=172.25.195.216:54001
OPEN SENDER UDP PORT=172.25.195.216:54002
delta_msec = 20
max_delay_msec = 1
Server Start: 172.25.195.216:50051
[UnityMemory] Configuration Parameters - Can be set up in boot.config
    "memorysetup-bucket-allocator-granularity=16"
    "memorysetup-bucket-allocator-bucket-count=8"
    "memorysetup-bucket-allocator-block-size=4194304"
    "memorysetup-bucket-allocator-block-count=1"
    "memorysetup-main-allocator-block-size=16777216"
    "memorysetup-thread-allocator-block-size=16777216"
    "memorysetup-gfx-main-allocator-block-size=16777216"
    "memorysetup-gfx-thread-allocator-block-size=16777216"
    "memorysetup-cache-allocator-block-size=4194304"
    "memorysetup-typetree-allocator-block-size=2097152"
    "memorysetup-profiler-bucket-allocator-granularity=16"
    "memorysetup-profiler-bucket-allocator-bucket-count=8"
    "memorysetup-profiler-bucket-allocator-block-size=4194304"
    "memorysetup-profiler-bucket-allocator-block-count=1"
    "memorysetup-profiler-allocator-block-size=16777216"
    "memorysetup-profiler-editor-allocator-block-size=1048576"
    "memorysetup-temp-allocator-size-main=4194304"
    "memorysetup-job-temp-allocator-block-size=2097152"
    "memorysetup-job-temp-allocator-block-size-background=1048576"
    "memorysetup-job-temp-allocator-reduction-small-platforms=262144"
    "memorysetup-allocator-temp-initial-block-size-main=262144"
    "memorysetup-allocator-temp-initial-block-size-worker=262144"
    "memorysetup-temp-allocator-size-background-worker=32768"
    "memorysetup-temp-allocator-size-job-worker=262144"
    "memorysetup-temp-allocator-size-preload-manager=262144"
    "memorysetup-temp-allocator-size-nav-mesh-worker=65536"
    "memorysetup-temp-allocator-size-audio-worker=65536"
    "memorysetup-temp-allocator-size-cloud-worker=32768"
    "memorysetup-temp-allocator-size-gfx=262144"
INFO: ACTIVATING ZUMOSIM-RUNNER
asset_name = zumosim-runner
config path = ./runtime/custom.json
delta_time_msec = 20
INFO: shmget() key=255 size=1129352 
Robot: ZumoRoboModel, PduWriter: ZumoRoboModel_actuator
channel_id: 1 pdu_size: 24
INFO: ZumoRoboModel create_lchannel: logical_id=1 real_id=0 size=24
WAIT START
Press ENTER to stop...
subscribe_pdu_channel: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: SubscribePduChannelRequest { asset_name: "UnityAsset", channel_id: 1, pdu_size: 24, listen_udp_ip_port: "172.25.192.1:54003", method_type: "UDP", robo_name: "ZumoRoboModel" }, extensions: Extensions }
create_asset_sub_pdu
create_pdu_channel: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: CreatePduChannelRequest { asset_name: "UnityAsset", channel_id: 0, pdu_size: 104, method_type: "UDP", robo_name: "ZumoRoboModel" }, extensions: Extensions }
INFO: shmget() key=255 size=1129352 
INFO: ZumoRoboModel create_lchannel: logical_id=0 real_id=1 size=104
create_asset_pub_pdu: robo_name=ZumoRoboModel channel_id=0 real_id=1
create_asset_pub_pdu: channel_ID=0
register: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: AssetInfo { name: "UnityAsset" }, extensions: Extensions }
asset_notification_start: Got a request: Request { metadata: MetadataMap { headers: {"te": "trailers", "content-type": "application/grpc", "user-agent": "grpc-csharp/2.37.0-dev grpc-c/15.0.0 (windows; chttp2)", "grpc-accept-encoding": "identity,deflate,gzip", "accept-encoding": "identity,gzip"} }, message: AssetInfo { name: "UnityAsset" }, extensions: Extensions }
```

また、Unityアプリが下図のように起動します。

![image](https://github.com/ncesnagoya/hakoniwa-zumosim/assets/164193/66bc9d79-1ef6-4c7e-a14c-b938e2aaaee1)

補足：起動時に、セキュリティ許可のダイアログが出ますが、「許可」してください。

注意：初回については、Windowsが本アプリケーションの実行を抑止していますので、一度だけ、`ZumoApp/model.exe`をダブルクリックして、実行を許可してください。起動したら終了してください。

### シミュレーション実行方法

Unityアプリケーションの`START`ボタンをクリックしてください。

また、終了する場合は、端末上で`ENTER` を実行してください。

注意：Unityのクローズしないようにしてください。

## docker image 保存とロード方法

最初に docker images で対象イメージを確認します。

```
% docker images
REPOSITORY                            TAG       IMAGE ID       CREATED          SIZE
ncesnagoya/hakoniwa-suzmosim-dev      v1.0.0    645e59a3440c   17 minutes ago   1.25GB
ncesnagoya/hakoniwa-suzmosim-run      v1.0.0    81fe7a69d1f8   29 minutes ago   3.72GB
```

### 保存方法
ここでは、`ncesnagoya/hakoniwa-suzmosim-run` を保存してみましょう。

```
 docker save ncesnagoya/hakoniwa-suzmosim-run -o zumosim-run.tar
```

`-o` オプションで、ファイル名を指定します。ファイルは tar ファイルで出力されますので、拡張子は `.tar` にします。

成功するとこうなります。

```
ls -lh
-rw-------@  1 tmori  staff   3.5G  8 23 14:06 zumosim-run.tar
```

### ロード方法

テストする場合は、一旦、保存した docker image　を削除しておきましょう。
その上で、以下のコマンドでロードできます。

```
% docker load -i ./zumosim-run.tar 
```

この通り、ロードできました。

```
% docker images
REPOSITORY                            TAG       IMAGE ID       CREATED          SIZE
ncesnagoya/hakoniwa-suzmosim-run      v1.0.0    81fe7a69d1f8   32 minutes ago   3.72GB
```

## 設計情報

![image](https://github.com/ncesnagoya/hakoniwa-zumosim/assets/164193/fd34d181-b083-4be8-9884-69130cefe04a)

