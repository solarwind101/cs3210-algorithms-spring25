import matplotlib.pyplot as plt
import seaborn as sns

sns.set_theme(style="darkgrid", palette="muted")

def plot_nT():
    ns, ts = [], []
    with open("nT_data", 'r') as f:
        for line in f:
            n, t = map(int, line.strip().split(','))
            ns.append(n)
            ts.append(t)

    plt.figure(figsize=(8, 6))
    plt.plot(ns, ts, marker='o', linestyle='-', color='royalblue', markersize=5, linewidth=2, label="T vs. n")

    plt.xlabel("n")
    plt.ylabel("T")
    plt.title("n vs. T", fontsize=12, fontweight="bold")
    plt.legend()
    plt.grid(True, linestyle="--", linewidth=0.5)

    plt.savefig("nT_plot.pdf", format="pdf", bbox_inches="tight")
    print("Saved as 'nT_plot.pdf'.")

if __name__ == "__main__":
    plot_nT()
