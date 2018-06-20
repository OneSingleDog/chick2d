# chick2d

![build](https://img.shields.io/teamcity/codebetter/bt428.svg)

## Overview 概览

A 2D "chicken dinner" game aimed for 2-5 players under the local area network.
<br>一款面向 2-5 人进行局域网对战的 2D “吃鸡”游戏。

Developed by **Cocos2d-x 3.16** and **Boost.Asio**.
<br>使用开源游戏框架 [**Cocos2d-x 3.16**](https://github.com/cocos2d/cocos2d-x) 和著名 C++ 网络库 **Boost.Asio** 进行开发。

## Build and Run 构建运行

Please download our [release version](https://github.com/OneSingleDog/chick2d/releases) firstly.
<br>请下载我们的[已发布版本](https://github.com/OneSingleDog/chick2d/releases)。

### Server 服务端

Run by cmd for Windows or Terminal for MacOS.
<br>Windows 系统下使用 cmd 运行，MacOS 系统下使用 Terminal 运行。

#### Optional operating parameters 可选运行参数

* `-PORT` The open port of the server, 8001 default.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 服务器的开放端口，默认 8001 端口。
			
* `-PLAYER` Number of players, 4 default, 2 at least and 5 at most.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 游戏人数，默认 4 人，最少 2 人，最多 5 人。
			
* `-BOX` Number of boxes (except airdrop), 20 default, 10 at least and 20 at most.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;初始箱子数量（不计空投），默认 20 个，最少 10 个，最多 20 个。

For example: `./chick2d_server -PORT 8001 -PLAYER 5 -BOX 15` for MacOS
<br>例如：在 MacOS 的 Terminal 中输入以上命令以启动服务端。

> Hint: Please make sure 'Local discovery' is open in the computer where the server application is running on.<br>
> 提示：确保运行服务端的计算机开启了“本机发现”。

> Hint: To build a loacl area network, just open your mobile phone hotspot.<br>
> 提示：如果想创建一个局域网，打开你的手机热点即可。

### Client 客户端

Firstly, make sure you and the server have connected to the same local area network.
<br>首先，确保你和服务器已经连接到同一个局域网中。

Then, double click to run the game.
<br>双击运行游戏。

Input your username, your server's IP and port.
<br>输入你的用户名、服务器的 IP 和端口号。

> Hint: Username can be no longer than 15 characters, you can input numbers, letters and '\_' only. By the way, because of the font we use, '\_' will show up as a space.<br>
> 提示：用户名长度不可以超过 15 个字符，且只允许输入数字和大小写字母。注意由于字体的缘故，下划线将以空格的形式显示。

Click 'Start' to begin your wonderful journey of chick2d!
<br>点击“开始”，然后享受你的 chick2d 之旅吧！

## Game Rules 游戏规则

### 游戏目标

消灭所有的其他玩家（红色名字），存活到最后即取得胜利。

### 游戏进程

#### 开始游戏

进入游戏后在地图上随机选择一个位置点击**空格**进行准备，准备后不可移动，右上角会显示已准备人数，当已准备人数达到游戏人数时，游戏自动开始。

游戏开始后，玩家使用 `wasd` 移动，按住左 `shift` 键可以跑步，移动速度加快，但此时不能进行换弹，射击，切枪等操作。

地图上有各种资源箱，足够靠近后可以按 `f` 打开箱子并对其中的物品进行拾取。

#### 药品

持有数量无上限，点击快捷键 `1-4` 以使用对应药品。此时血条旁边会出现一个治疗中的标志，此时移动，开枪，换弹，切枪等操作都会中断这个状态，在等待一段时间后，这个标志会消失，消耗药品并恢复对应的血量。

1. 医疗箱，回复所有生命值。
2. 急救包，将生命值回复至75%，只有生命值低于75%时可以使用。
3. 能量饮料，回复10%的最大生命值，任何时间可以使用，最多回满。
4. 绷带，回复10%的最大生命值，只用生命值低于75%时可以使用，最多回复至75%。
	
#### 防弹衣

抵消受到武器伤害的40%，并失去同等数值的耐久。

分两种防弹衣，普通箱子中的防弹衣最大耐久 20，空投中出现的防弹衣最大耐久 100。

#### 武器

最多持有两件武器，分主武器与服务器。按 `Q` 键切换主副武器，切换武器会中止换弹

可以在状态栏查看当前子弹数与后备子弹数，按 `R` 键换子弹。

拾取武器时如果主武器为空拾取到主武器的位置上，否则如果副武器为空拾取到副武器的位置上，如果已持有两把武器，会交换当前主武器与选择拾取的武器。

任何武器被拾取时当前弹量都是0，需要进行换弹操作。

武器拥有射程与射击角度范围，每当点击鼠标左键时，如果不在跑步状态，换弹状态且主武器有子弹，将会开枪，子弹将会在射击角度范围内随机选择一个角度发出，直到打到人或墙或超出射程为止。

武器的射程与射击角度可以在游戏中清晰地看到。

每把武器都有各自的特点，根据不同情况选择适合自己的武器能事半功倍哦！

1. 狙击枪：射击精度很高，伤害高，射程远，同时在持有狙击枪时，可以点击**右键**进入瞄准镜状态，此时视野会向鼠标方向前推，这个状态下可以进行射击，但不能进行移动，再点击右键或移动会取消这个状态。
2. 霰弹枪：射程短，射击散射角度大，但每次开枪，会在射击角度内随机生成5发小弹丸，一个敌人可以被多发弹丸命中。
3. 自动步枪：射速快，射程中，射击散射角度较低。
4. 手枪：射速，射程与射击散射角度都比较一般。

当你开枪成功时能够看到枪口的火光并听到音效，如果没有，请检查自己的弹夹，或松开左 `shift` 键，退出跑步状态。

#### 地图和毒圈

按 `M` 键可以查看小地图，上面会显示自己的位置，毒圈的位置，以及空投的位置，再次按下 `M` 键，小地图会消失。

安全区每**一分钟**缩小一次，在安全区外会持续掉血，并在主武器信息旁出现警示标志，毒圈每缩小一次，毒圈的伤害就会增加，缩小后的毒圈范围一定在原先的毒圈范围内。

每当缩小毒圈时，会出现一个空投箱子，资源十分丰富，并在小地图上标识。去追求梦想吧！

#### 其他

当你被击中时，你的身上会出现血雾，并听到音效，这时请注意隐蔽。

这个游戏有战争迷雾，你只能看到你的人物相对于鼠标方向的敌人，另一方向的敌人将会隐藏在一片迷雾中。

但任何时候敌人开枪，你将看到一圈圈波纹从其开枪的地方出现，去寻找他们并干掉他们吧。
	
希望你们能够享受这个游戏。大吉大利，chick2d！

<!--
## Reference link

[Document of cocos2d-x](http://docs.cocos.com/cocos2d-x/manual/zh/)

[cocos2d-x API](http://www.cocos2d-x.org/docs/api-ref/cplusplus/v3x/)

[Introduce and brief Guide for Boost.Asio](https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter1.html)

[teeworlds - A famous shooting game on Github](https://github.com/teeworlds/teeworlds)
-->

## Developer 开发者

[Junkai Liang](https://github.com/liangjk)

[Guanyue Bian](https://github.com/Mangxin)

[Yupeng Hou](https://github.com/hyp1231)

[Ningxuan Feng](https://github.com/Whitycatty)

## Further More 仍需完善

* Same name conflict of different users
<br>一局游戏中的名字冲突问题

* English translation of Game Rules
<br>游戏规则的英文翻译

* Icon for chick2d
<br>游戏图标

* chick2d for Linux
<br>在 Linux 上游玩 chick2d！

Input your username, your server's IP and port.
<br>输入你的用户名、服务器的 IP 和端口号。

> Hint: Username can be no longer than 15 characters, you can input numbers, letters and '\_' only. By the way, because of the font we use, '\_' will show up as a space.<br>
> 提示：用户名长度不可以超过 15 个字符，且只允许输入数字和大小写字母。注意由于字体的缘故，下划线将以空格的形式显示。

Click 'Start' to begin your wonderful journey of chick2d!
<br>点击“开始”，然后享受你的 chick2d 之旅吧！

## Game Rules 游戏规则

### 游戏目标

消灭所有的其他玩家（红色名字），存活到最后即取得胜利。

### 游戏进程

#### 开始游戏

进入游戏后在地图上随机选择一个位置点击**空格**进行准备，准备后不可移动，右上角会显示已准备人数，当已准备人数达到游戏人数时，游戏自动开始。

游戏开始后，玩家使用 `wasd` 移动，按住左 `shift` 键可以跑步，移动速度加快，但此时不能进行换弹，射击，切枪等操作。

地图上有各种资源箱，足够靠近后可以按 `f` 打开箱子并对其中的物品进行拾取。

#### 药品

持有数量无上限，点击快捷键 `1-4` 以使用对应药品。此时血条旁边会出现一个治疗中的标志，此时移动，开枪，换弹，切枪等操作都会中断这个状态，在等待一段时间后，这个标志会消失，消耗药品并恢复对应的血量。

1. 医疗箱，回复所有生命值。
2. 急救包，将生命值回复至75%，只有生命值低于75%时可以使用。
3. 能量饮料，回复10%的最大生命值，任何时间可以使用，最多回满。
4. 绷带，回复10%的最大生命值，只用生命值低于75%时可以使用，最多回复至75%。
	
#### 防弹衣

抵消受到武器伤害的40%，并失去同等数值的耐久。

分两种防弹衣，普通箱子中的防弹衣最大耐久 20，空投中出现的防弹衣最大耐久 100。

#### 武器

最多持有两件武器，分主武器与服务器。按 `Q` 键切换主副武器，切换武器会中止换弹

可以在状态栏查看当前子弹数与后备子弹数，按 `R` 键换子弹。

拾取武器时如果主武器为空拾取到主武器的位置上，否则如果副武器为空拾取到副武器的位置上，如果已持有两把武器，会交换当前主武器与选择拾取的武器。

任何武器被拾取时当前弹量都是0，需要进行换弹操作。

武器拥有射程与射击角度范围，每当点击鼠标左键时，如果不在跑步状态，换弹状态且主武器有子弹，将会开枪，子弹将会在射击角度范围内随机选择一个角度发出，直到打到人或墙或超出射程为止。

武器的射程与射击角度可以在游戏中清晰地看到。

每把武器都有各自的特点，根据不同情况选择适合自己的武器能事半功倍哦！

1. 狙击枪：射击精度很高，伤害高，射程远，同时在持有狙击枪时，可以点击**右键**进入瞄准镜状态，此时视野会向鼠标方向前推，这个状态下可以进行射击，但不能进行移动，再点击右键或移动会取消这个状态。
2. 霰弹枪：射程短，射击散射角度大，但每次开枪，会在射击角度内随机生成5发小弹丸，一个敌人可以被多发弹丸命中。
3. 自动步枪：射速快，射程中，射击散射角度较低。
4. 手枪：射速，射程与射击散射角度都比较一般。

当你开枪成功时能够看到枪口的火光并听到音效，如果没有，请检查自己的弹夹，或松开左 `shift` 键，退出跑步状态。

#### 地图和毒圈

按 `M` 键可以查看小地图，上面会显示自己的位置，毒圈的位置，以及空投的位置，再次按下 `M` 键，小地图会消失。

安全区每**一分钟**缩小一次，在安全区外会持续掉血，并在主武器信息旁出现警示标志，毒圈每缩小一次，毒圈的伤害就会增加，缩小后的毒圈范围一定在原先的毒圈范围内。

每当缩小毒圈时，会出现一个空投箱子，资源十分丰富，并在小地图上标识。去追求梦想吧！

#### 其他

当你被击中时，你的身上会出现血雾，并听到音效，这时请注意隐蔽。

这个游戏有战争迷雾，你只能看到你的人物相对于鼠标方向的敌人，另一方向的敌人将会隐藏在一片迷雾中。

但任何时候敌人开枪，你将看到一圈圈波纹从其开枪的地方出现，去寻找他们并干掉他们吧。
	
希望你们能够享受这个游戏。大吉大利，chick2d！

<!--
## Reference link

[Document of cocos2d-x](http://docs.cocos.com/cocos2d-x/manual/zh/)

[cocos2d-x API](http://www.cocos2d-x.org/docs/api-ref/cplusplus/v3x/)

[Introduce and brief Guide for Boost.Asio](https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter1.html)

[teeworlds - A famous shooting game on Github](https://github.com/teeworlds/teeworlds)
-->

## Developer 开发者

[Junkai Liang](https://github.com/liangjk)

[Guanyue Bian](https://github.com/Mangxin)

[Yupeng Hou](https://github.com/hyp1231)

[Ningxuan Feng](https://github.com/Whitycatty)

## Further More 仍需完善

* Same name conflict of different users
<br>一局游戏中的名字冲突问题

* English translation of Game Rules
<br>游戏规则的英文翻译

* Icon for chick2d
<br>游戏图标

* chick2d for Linux
<br>在 Linux 上游玩 chick2d！
