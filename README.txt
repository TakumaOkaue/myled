０～５の数字に対応した数のledを光らせるプログラム。
ラズパイ3 B　で製作。

～使い方～
myled.c Makefile　の二つをダウンロードし、適当な同じフォルダに入れる。
フォルダにいれたら、
$ make
とコマンドを打ち込み読み込む。成功したら動く。
使用するGPIOピンは20,21,22,23,24,25番のもの。順番にledにつなげる。
起動方法は以下のコマンドを打つ
$ sudo insmod myled.ko
$ sudo chmod 666 /dev/myled0 
これで準備完了。

$ echo number > /dev/myled0 
numberのところに好きな数字を入れると数字に対応した数のledが光る。

終了する際は以下のコマンドを打つ。

$ sudo rm /dev/myled0 
$ sudo rmmod myled$ sudo rmmod myled
