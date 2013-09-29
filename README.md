footoff_october
===========

##目次
- 環境構築
	- Xcode
	- node.js
	- npm
	- Git 初期設定
	- リポジトリのクローン
- 実行
- スマホでの実行
- エラー対処
	- serialportエラー
	- 端末の未検出エラー
- 参考


## 環境構築
Macを想定しています。

### Xcode
AppleのIDEであるXcodeをインストールしてください。

[Xcode](https://developer.apple.com/xcode/)

### node.js
サーバサイドJavascriptライブラリ[node.js](http://nodejs.org/)をインストールしてください。

Homebrewの場合

	$ brew install node

MacPortsの場合

	$ sudo port install nodejs


### npm
https://npmjs.org/


	$ curl http://npmjs.org/install.sh | sh

shの前にsudoがいる場合もあるみたいです。

	$ curl http://npmjs.org/install.sh | sudo sh


### Git 初期設定
[Git - Gitのインストール]
(http://git-scm.com/book/ja/%E4%BD%BF%E3%81%84%E5%A7%8B%E3%82%81%E3%82%8B-Git%E3%81%AE%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB)
(Xcodeのcommand toolsでもインストールできます。)

Github.comに登録しているuser.nameとuser.emailを設定してください。

	$ git config --global user.name "your name"
	$ git config --global user.email your@ma.il


### リポジトリのクローン
作業ディレクトリ上で以下のコマンドを叩いてください。


	$ git clone git@github.com:yutaono/footoff_october.git

または、

	$ git clone https://github.com/yutaono/footoff_october.git


無事にクローンされればカレントディレクトリにfootoff_octoberディレクトリができています。

	$ cd footoff_october


package.jsonからモジュールをインストールします。

	$ sudo npm install


## 実行

以下のコマンドでサーバを起動します。

	$ node app

ブラウザで、http://localhost:3000にアクセス。

## スマホでの実行
スマホとMacが同一LAN内にあるとします。

Macのターミナルで、

	$ ifconfig

を実行し、en1: の要素中のinetのアドレス(例:127.0.0.1)が表示されます。サーバを起動してから、そのアドレスに:3000をつけたアドレス(例:127.0.0.1:3000)に、スマホからアクセスします。



## エラー対処

### serialportエラー

	$ node app

	/home/username/node_serial_demo/node_modules/serialport/node_modules/bindings/bindings.js:83
	        throw e
	              ^
	Error: /home/username/tmp/node_serial_demo/node_modules/serialport/build/Release/serialport.node: invalid ELF header


上記のエラーがでた場合、footoff_octoberディレクトリ上で

	$ npm uninstall serialport
	$ npm install serialport

を実行してください。

### 端末の未検出エラー

	$ node app.js
	   info  - socket.io started
	Express server listening on port 3000
	err Error: Cannot open /dev/tty.usbmodemfd131

上記のエラーはArduino等の端末が見つからないときに起こります。

	$ ls /dev/tty*

または

	$ ls /dev/tty.usb*

で表示されるポート名をapp.jsの

	var portName = '/dev/tty.usbmodemfd131';

に書き換えてください。

## 参考
[node-serialportを使ってNode.jsでArduinoと通信する - 人と技術のマッシュアップ]
(http://tomowatanabe.hatenablog.com/entry/2013/03/23/233554)
