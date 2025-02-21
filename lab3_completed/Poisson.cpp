#include <dolfin.h>
#include "Poisson.h"

using namespace dolfin;

// Source term (right-hand side)
class Source : public Expression
{
	void eval(Array<double>& values, const Array<double>& x) const
	{
		double dx = x[0];
		double dy = x[1];
		if (abs(dx*dx + dy*dy - 0.2025) < 0.01) {
			values[0] = 50;
		}
		else {
			values[0] = 0;
		}
	}
};

// Normal derivative (Neumann boundary condition)
class dUdN : public Expression
{
	void eval(Array<double>& values, const Array<double>& x) const
	{
		values[0] = 0;
	}
};

// Sub domain for Dirichlet boundary condition
class DirichletBoundary : public SubDomain
{
	bool inside(const Array<double>& x, bool on_boundary) const
	{
		return (abs(x[0]*x[0] + x[1]*x[1] - 0.16)) < 0.0001 or (abs(x[0]*x[0] + x[1]*x[1] - 0.25)) < 0.0001;
	}
};

int main()
{
	// Create mesh and function space
	auto mesh = std::make_shared<Mesh>("../holymoly.xml");
	auto V = std::make_shared<Poisson::FunctionSpace>(mesh);

	// Define boundary condition
	auto u0 = std::make_shared<Constant>(0.0);
	auto boundary = std::make_shared<DirichletBoundary>();
	DirichletBC bc(V, u0, boundary);

	// Define variational forms
	Poisson::BilinearForm a(V, V);
	Poisson::LinearForm L(V);
	auto f = std::make_shared<Source>();
	auto g = std::make_shared<dUdN>();
	L.f = f;
	L.g = g;

	// Compute solution
	Function u(V);
	solve(a == L, u, bc);

	// Save solution in VTK format
	File file("poisson.pvd");
	file << u;
	return 0;
}
