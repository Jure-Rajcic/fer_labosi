import numpy as np
import matplotlib.pyplot as plt


n = 200
distribution = 500

A = np.diag(2 * np.ones(n) * n, 0) + np.diag(-1 * np.ones(n-1) * n, 1) + np.diag(-1 * np.ones(n-1) * n, -1)
f = [((1 + i / n) / n) for i in range(1, n+1)]
α = np.linalg.solve(A, f)

x = np.linspace(1, 2, distribution)
arr = np.zeros(distribution)
for i in range(n):
    middle = 1 + (i + 1) / (n + 1)
    left = middle - 1 / (n + 1)
    right = middle + 1 / (n + 1)
    j = 0
    for distance in x:
        if distance >= left and distance <= middle:
            arr[j] += (distance - left) * n * α[i]
        elif distance > middle and distance <= right:
            arr[j] += (right - distance) * n * α[i]
        j += 1

plt.plot(x, arr, 'r')
plt.plot(x, -x ** 3 / 6 + 7 / 6 * x - 1, color='blue')
plt.show()
