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

## ビルド方法

アプリケーションは以下の手順でビルドします。

```
% bash docker/build.bash <aplname>
```

`<aplname>` は、ビルド対象とするアプリケーション名です。

TODO: <aplname> に対応するアプリケーションをビルド環境にどうやって取り込むか要検討

## 箱庭起動方法

以下のコマンドを実行することで、箱庭を起動できます。

補足：アプリケーションのシミュレーション実行開始のトリガは、Unity側で行います。

```
% bash docker/run.bash 
```

成功するとこなります。

```
x86_64
wsl2
INFO: ACTIVATING HAKO-CONDUCTOR
OPEN RECIEVER UDP PORT=172.25.195.216:54001
OPEN SENDER UDP PORT=172.25.195.216:54002
delta_msec = 20
max_delay_msec = 100
INFO: shmget() key=255 size=1129352
Server Start: 172.25.195.216:50051
INFO: ACTIVATING ZUMOSIM-RUNNER
asset_name = zumosim-runner
config path = ./runtime/custom.json
delta_time_msec = 20
INFO: shmget() key=255 size=1129352
Robot: ZumoRoboModel, PduWriter: ZumoRoboModel_actuator
channel_id: 1 pdu_size: 24
INFO: ZumoRoboModel create_lchannel: logical_id=1 real_id=0 size=24
```

終了する場合は、`CTRL+C` を実行してください。

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

