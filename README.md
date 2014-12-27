Minesweeper
===========

作者: 平井芳孝  
作成日: 2014年12月28日  

#### 1. Build  
minesweeper.cpp および minesweeper.h は
g++ (GCC) 4.8.2において以下のようなコマンドで  
ビルドできます。  

```
$ g++ -Wall -O2 -std=c++11 
```

#### 2. Run  
コマンドライン引数に*0*を渡すと*通常モード*で
*1*を渡すと*チートモード*でゲームを開始します。  
このチートモードとはフィールドの地雷がどこにあるのかを
実際のフィールドの真下に表示するモードです。  
```
./a.out 1または0
```

