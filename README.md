# hakoniwa-zumosim


## ビルド方法

```
% bash docker/build.bash <aplname>
```

現時点では、この後、以下のコマンドを実行する必要があります。

```
# bash clean.bash ; bash build.bash 
```

## 箱庭起動方法

```
% bash docker/run.bash 
```

現時点では、この後、以下のコマンドを実行する必要があります。

```
# bash run.bash
```

成功するとこなります。

```
INFO: ACTIVATING HAKO-CONDUCTOR
OPEN RECIEVER UDP PORT=127.0.0.1:54001
OPEN SENDER UDP PORT=127.0.0.1:54002
delta_msec = 20
max_delay_msec = 100
INFO: shmget() key=255 size=1129352 
Server Start: 127.0.0.1:50051
INFO: ACTIVATING ZUMOSIM-RUNNER
asset_name = zumosim-runner
config path = ./runtime/custom.json
delta_time_msec = 20
INFO: shmget() key=255 size=1129352 
Robot: EV3TrainModelWithBaggage, PduWriter: EV3TrainModelWithBaggage_ev3_sensor
channel_id: 1 pdu_size: 248
INFO: EV3TrainModelWithBaggage create_lchannel: logical_id=1 real_id=0 size=248
Robot: EV3CrossingGateModel, PduWriter: EV3CrossingGateModel_ev3_sensor
channel_id: 1 pdu_size: 248
INFO: EV3CrossingGateModel create_lchannel: logical_id=1 real_id=1 size=248
WAIT START
```

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


