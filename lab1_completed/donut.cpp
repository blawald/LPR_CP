#include <gmsh.h>
#include <vector>
#include <string>

int main(int argc, char **argv) {


	//quick overview:	
	//	
	//Completing the doughnut-making lab was a game-changer for me! At first, I didn’t expect much more than a coding exercise, 
	//but it turned out to be a brilliant deep dive into geometry and simulation. Creating tori helped me really get mesh generation and 3D 
	//modeling, which I know will be invaluable in my future research. The problem-solving challenges were super engaging, and figuring out 
	//why my shapes didn’t extrude correctly taught me a ton about symmetry and practical computational techniques. This lab not only boosted 
	//my technical skills but also sparked creative thinking—honestly, it made math and science feel more alive than ever. Highly recommend!


	gmsh::initialize();
	gmsh::model::add("donut");

	double lc = 4e-2;
	double L = 0.05; 
	double shift = 1;

	double radius_big = 0.5;
	double radius_small = 0.4;

	int center = gmsh::model::geo::addPoint(shift, 0, 0, lc);	// center
	int big_p1 = gmsh::model::geo::addPoint(shift+radius_big, 0, 0, lc); 
	int big_p2 = gmsh::model::geo::addPoint(shift, radius_big, 0, lc); 
	int big_p3 = gmsh::model::geo::addPoint(shift-radius_big, 0, 0, lc); 
	int big_p4 = gmsh::model::geo::addPoint(shift, -radius_big, 0, lc); 

	int big_arc1 = gmsh::model::geo::addCircleArc(big_p1, center, big_p2);
	int big_arc2 = gmsh::model::geo::addCircleArc(big_p2, center, big_p3);
	int big_arc3 = gmsh::model::geo::addCircleArc(big_p3, center, big_p4);
	int big_arc4 = gmsh::model::geo::addCircleArc(big_p4, center, big_p1);

	int curveLoop_big = gmsh::model::geo::addCurveLoop({big_arc1, big_arc2, big_arc3, big_arc4});

	int small_p1 = gmsh::model::geo::addPoint(shift+radius_small, 0, 0, lc); 
	int small_p2 = gmsh::model::geo::addPoint(shift, radius_small, 0, lc); 
	int small_p3 = gmsh::model::geo::addPoint(shift-radius_small, 0, 0, lc); 
	int small_p4 = gmsh::model::geo::addPoint(shift, -radius_small, 0, lc); 

	int small_arc1 = gmsh::model::geo::addCircleArc(small_p1, center, small_p2);
	int small_arc2 = gmsh::model::geo::addCircleArc(small_p2, center, small_p3);
	int small_arc3 = gmsh::model::geo::addCircleArc(small_p3, center, small_p4);
	int small_arc4 = gmsh::model::geo::addCircleArc(small_p4, center, small_p1);

	int curveLoop_small = gmsh::model::geo::addCurveLoop({small_arc1, small_arc2, small_arc3, small_arc4});

	int circle = gmsh::model::geo::addPlaneSurface({curveLoop_big, -curveLoop_small});

	
	std::vector<std::pair<int, int> > ov;
	gmsh::model::geo::revolve({{2, circle}}, 0, 0, 0, 0, 1, 0, M_PI * (2-1e-6), ov,
                              {50}); // Setting angle to 2*Pi cause some problems, so I am using (2-10^-6) Pi instead (obviousness level = 0/10) 
	gmsh::model::geo::removeAllDuplicates();

	gmsh::model::geo::synchronize();

	gmsh::model::mesh::generate(3);

	gmsh::write("donut.msh");

	std::vector<std::string> args(argv, argv + argc);
	if(std::find(args.begin(), args.end(), "-nopopup") == args.end())
		gmsh::fltk::run();

	gmsh::finalize();
	return 0;
}

