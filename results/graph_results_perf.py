import csv
import math
import numpy as np
import matplotlib.pyplot as plt

CSV_FILE = "./results/results_perf.csv"

q_values = []
times = []

with open(CSV_FILE, newline='') as csvfile:
    reader = csv.reader(csvfile)

    # Lignes 1 et 2 : paramètres
    param_names = next(reader)
    param_values = next(reader)
    param_str = ", ".join(f"{n}={v}" for n, v in zip(param_names, param_values))

    for row in reader:
        if len(row) < 4:
            continue

        q_str, a_str, b_str, time_str = row

        try:
            q = int(q_str)
            t = float(time_str)
        except ValueError:
            continue

        if q > 1 and t > 0:
            q_values.append(q)
            times.append(t)

q_values = np.array(q_values, dtype=float)
times = np.array(times, dtype=float)

loglogq = np.log(np.log(q_values))

# Régression linéaire
coeffs = np.polyfit(loglogq, np.log(times), 1)
slope, intercept = coeffs

# Droite de régression
x_fit = np.linspace(loglogq.min(), loglogq.max(), 200)
y_fit = np.exp(intercept + slope * x_fit)

# Création du graphique
plt.figure(figsize=(8, 5))

plt.scatter(loglogq, times, alpha=0.7, label="Résultats de test-perf")
plt.plot(x_fit, y_fit, '--', color="orange", label=fr"Régression linéaire (pente = {slope:.4f})")

plt.yscale('log')

plt.xlabel(r"$\log(\log q)$")
plt.ylabel("Temps d'exécution (s)")
plt.title(f"Temps d'exécution de l'algorithme de Schoof, échelle logarithmique.\n{param_str}")
plt.grid(True, which="both", linestyle="--", alpha=0.6)
plt.legend()

plt.tight_layout()
plt.show()