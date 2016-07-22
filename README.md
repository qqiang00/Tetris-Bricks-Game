# Tetris-Bricks-Game
A simple Tetris game with a LCD like display on windows system.  C++ with Win32 API

俄罗斯方块是一个经典的游戏。大学的时候曾经用TurboC在dos下用双缓存技术加硬件中断处理设计了一个很漂亮的俄罗斯方块，当时还是存储在一个软盘里。几经更换电脑，把这个经典的代码弄丢了。现在闲来，重写一个。这个项目主要想以LCD阵列的形式完成俄罗斯方块游戏的核心算法，使用C++调用Win32 API。

1.项目有几个核心的类：GraphicsManager类负责绘图;Brick类设计是组成一个完整运动的俄罗斯方块的小方块（简单俄罗斯方块都是有四个小方块组成的，brick就是描述这四个小方块。Bricks类（复数）指一个完整运动的方块单元。GameControl类负责逻辑控制。BricksGame类包含winmain函数，负责交互。

2.个人觉得俄罗斯方块游戏最大的难点在于方块的边界检测，以便判断后续动作（是否因边界或其他方块而运动受限，是否已不能继续下降）。

3.有一个值得思考的问题：当新的方块生成后进入主游戏区域的过程中但是还没有完全进入时，是否允许此方块旋转？不同的设定要求不一样的设计，代码里的设计是允许方块未完全进入游戏区时旋转的，我们仅是把主游戏区的数组数据结构向上扩展了4行，同可见的主游戏区作为一个整体管理。当有部分小方块还未落入主游戏区时，我们需要做的仅仅是不绘制它。这省了很多事情。

4.游戏仅是实现了关键部分，可以设置掉落速度，但没有设计积分系统，还有很多可以完善的地方，其中之一是在试图旋转方块时，当方块位于左右两边紧邻边界时，虽然另一侧有足够的空间，但并不能顺利旋转。我考察过一些类似的程序处理，有些和我的处理机制一样，有些则更加优化，只要单侧有足够的空间就允许旋转。

5.代码未经优化，仅供参考。

