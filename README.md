# 五子棋 (Wuziqi)

这是一个使用 Qt Widgets (QMake) 实现的五子棋项目的完整实现分支。

功能:
- 本地人人对战
- 简单人机（启发式评估）
- 悔棋/重做
- 存档/加载棋谱
- 胜利判定与高亮

构建:
- 需要 Qt 5 或 Qt 6（带 Qt Widgets）
- 在项目根目录运行 qmake && make 或使用 Qt Creator 打开 wuziqi.pro

文件:
- 主程序: main.cpp
- UI: src/MainWindow.*
- 棋盘与交互: src/GameBoard.*
- 游戏规则: src/GameLogic.*
- AI: src/AIPlayer.*

我已经把代码提交到分支 feature/full-implementation。请在该分支上测试并告知你想要的进一步改进（比如增加回放控件、AI 深度搜索、网络对战或国际化）。
