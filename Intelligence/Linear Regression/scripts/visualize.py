"""读取 C++ 导出的 CSV，绘制训练 loss 和最终预测值。"""

from pathlib import Path
import argparse

import matplotlib.pyplot as plt
import pandas as pd


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("results", type=Path, help="C++ 输出的结果目录")
    args = parser.parse_args()

    history = pd.read_csv(args.results / "loss_history.csv")
    predictions = pd.read_csv(args.results / "predictions.csv")

    figure, (loss_axis, prediction_axis) = plt.subplots(1, 2, figsize=(13, 5))

    # 每个学习率一条曲线：可以观察下降速度以及是否发散。
    for learning_rate, group in history.groupby("learning_rate"):
        loss_axis.plot(group["epoch"], group["loss"], label=f"lr={learning_rate:g}")
    loss_axis.set_title("Training loss")
    loss_axis.set_xlabel("Epoch")
    loss_axis.set_ylabel("Half MSE")
    # 用对数纵轴看清楚“先快速下降、后慢慢收敛”的阶段。
    loss_axis.set_yscale("log")
    loss_axis.grid(alpha=0.3)
    loss_axis.legend()

    prediction_axis.plot(
        predictions["sample"], predictions["target"], "o-", label="target"
    )
    for column in predictions.columns[2:]:
        prediction_axis.plot(predictions["sample"], predictions[column], "--", label=column)
    prediction_axis.set_title("Predictions on the training set")
    prediction_axis.set_xlabel("Sample index")
    prediction_axis.set_ylabel("Target")
    prediction_axis.grid(alpha=0.3)
    prediction_axis.legend(fontsize="small")

    figure.tight_layout()
    output = args.results / "training_visualization.png"
    figure.savefig(output, dpi=150)
    print(f"saved: {output}")
    plt.show()


if __name__ == "__main__":
    main()
