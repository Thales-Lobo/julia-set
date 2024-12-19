import pandas as pd
import matplotlib.pyplot as plt

# Global Variables for file paths
PARALLEL_CSV_PATH = "../parallel/report/parallel_execution_report.csv"
SEQUENTIAL_CSV_PATH = "../sequential/report/sequential_execution_report.csv"
GRAPH_PATH = "../analysis/graphs"


def plot_execution_time(csv_path: str) -> None:
    """
    Reads the parallel execution data from the CSV file and generates a plot of execution time vs resolution
    for different numbers of processes, filtered by specific resolutions.

    Args:
        csv_path (str): Path to the CSV file with the parallel execution data.
    """
    # Read the CSV file
    data = pd.read_csv(csv_path)

    # Filter data for specific resolutions (500, 1000, 5000)
    specific_resolutions = [500, 1000, 5000]
    data_filtered = data[data["Resolution"].isin(specific_resolutions)]

    # Filter data for specific process counts
    process_counts = [2, 4, 8, 16]
    plt.figure(figsize=(10, 6))
    for process in process_counts:
        subset = data_filtered[data_filtered["Processes"] == process]
        plt.plot(subset["Resolution"], subset["ExecutionTime"], marker='o', label=f"{process} Processes")

    # Add labels, title, and legend
    plt.xlabel("Resolution (Number of Pixels)", fontsize=12)
    plt.ylabel("Execution Time (Seconds)", fontsize=12)
    plt.title("Execution Time vs Resolution for Different Process Counts", fontsize=14)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Save and show the plot
    plt.savefig(f"{GRAPH_PATH}/execution_time_vs_resolution_filtered.png")
    plt.show()


def plot_speedup(parallel_csv: str, sequential_csv: str) -> None:
    """
    Reads parallel and sequential execution data, calculates speedup, and generates a plot of speedup vs number of processes
    for different resolutions from the parallel execution data.

    Args:
        parallel_csv (str): Path to the CSV file with the parallel execution data.
        sequential_csv (str): Path to the CSV file with the sequential execution data.
    """
    # Read both CSV files
    parallel_data = pd.read_csv(parallel_csv)
    sequential_data = pd.read_csv(sequential_csv)

    # Merge parallel data with sequential data based on Resolution
    merged_data = pd.merge(parallel_data, sequential_data, on="Resolution", suffixes=("_parallel", "_sequential"))

    # Calculate speedup: sequential time divided by parallel time
    merged_data["Speedup"] = merged_data["ExecutionTime_sequential"] / merged_data["ExecutionTime_parallel"]

    # Define process counts to consider for the X-axis
    process_counts = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]  # Possible process values

    # Create a figure for plotting
    plt.figure(figsize=(10, 6))

    # Loop through each resolution and plot the speedup vs processes
    for resolution in merged_data["Resolution"].unique():
        subset = merged_data[merged_data["Resolution"] == resolution]

        # Plot speedup vs processes for this resolution
        plt.plot(
            subset["Processes_parallel"], 
            subset["Speedup"], 
            marker='o', 
            label=f"Resolution {resolution}"
        )

    # Add labels, title, and legend
    plt.xlabel("Number of Processes", fontsize=12)
    plt.ylabel("Speedup", fontsize=12)
    plt.title("Speedup vs Number of Processes for Different Resolutions", fontsize=14)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Save and show the plot
    plt.savefig(f"{GRAPH_PATH}/speedup_vs_processes.png")
    plt.show()


def plot_execution_time_vs_processes_for_resolution(csv_path: str, resolution: int = 4000) -> None:
    """
    Reads the parallel execution data from the CSV file and generates a plot of execution time vs number of processes
    for a specific resolution (default is resolution = 4000).

    Args:
        csv_path (str): Path to the CSV file with the parallel execution data.
        resolution (int): The resolution value to filter the data for (default is 4000).
    """
    # Read the CSV file
    data = pd.read_csv(csv_path)

    # Filter the data for the specific resolution
    data_filtered = data[data["Resolution"] == resolution]

    # Plot execution time vs processes for the given resolution
    plt.figure(figsize=(10, 6))
    plt.plot(data_filtered["Processes"], data_filtered["ExecutionTime"], marker='o', label=f"Resolution = {resolution}")

    # Add labels, title, and legend
    plt.xlabel("Number of Processes", fontsize=12)
    plt.ylabel("Execution Time (Seconds)", fontsize=12)
    plt.title(f"Execution Time vs Number of Processes for Resolution = {resolution}", fontsize=14)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Save and show the plot
    plt.savefig(f"{GRAPH_PATH}/execution_time_vs_processes_for_resolution_{resolution}.png")
    plt.show()


if __name__ == "__main__":
    # Generate the plots
    plot_execution_time(PARALLEL_CSV_PATH)
    plot_speedup(PARALLEL_CSV_PATH, SEQUENTIAL_CSV_PATH)
    plot_execution_time_vs_processes_for_resolution(PARALLEL_CSV_PATH, resolution=4000)
