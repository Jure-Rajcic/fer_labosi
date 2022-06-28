# Poissonov problem u 2D, P1 elementi
# -(u_xx + u_yy) = -6, na 0<x<1, 0<y<1
# u_D = 1+x^2+4y^2, na rubu

# Koristene varijable
#   mesh = diskretizacija domene
#   V = prostor konacnih elemenata
#   u_R = rubni uvjet za rjesenje
#   bc = rubni uvjet interpoliran u prostor konacnih elemenata
#   a = lijeva strana slabe formulacije
#   A = sastavljena matrica lijeve strane
#   L = desna strana slabe formulacije
#   b = sastavljen vektor desne strane
#   u = funkcija rjesenja
import fenics as fn
import numpy as np
import matplotlib.pyplot as plt


# Diskretizacija jedinicnog intervala i definiranje
# prostora konacnih elemenata na diskretizaciji
mesh = fn.RectangleMesh(fn.Point(1, 2), fn.Point(2, 3),50,50) #nx,ny
V = fn.FunctionSpace(mesh, 'CG', 1)

# Define boundary condition
u_D = fn.Expression('0', degree=1)

def boundary(x, on_boundary):
	return on_boundary

bc = fn.DirichletBC(V, u_D, boundary)

# Slaba formulacija
u = fn.TrialFunction(V)
v = fn.TestFunction(V)
f = fn.Expression('x[0] * x[1]', degree=2)
a = fn.dot(fn.grad(u), fn.grad(v))*fn.dx
L = f*v*fn.dx

# Kreiranje matrice sustava i vektora desne strane
A = fn.assemble(a)
b = fn.assemble(L)

bc.apply(A, b)

# Rjesavanje sustava linearnih jednadzbi
u = fn.Function(V)
U = u.vector()
fn.solve(A, U, b)

# Grafovi
fig1 = plt.figure()
fn.plot(mesh)
plt.savefig('mesh2D_zad2.png')

fig2 = plt.figure()
fn.plot(u)
plt.savefig('zad2.png')

# File za paraview
vtkfile = fn.File('poisson2D.pvd')
vtkfile << u
