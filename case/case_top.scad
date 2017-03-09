include <rounded_cube.scad>
include <dimensions.scad>

module case_base(){
    rounded_cube(case_width,case_length,case_height,3);
    translate([0,0,case_height]) rounded_cube(case_width, case_length, 2, 3);
};

module case_cuts(){
    translate([wall_thickness, wall_thickness,wall_thickness]) cube([pcb_width, pcb_length, case_height]);
    translate([1.25,1.25,case_height]) rounded_cube(case_width-2*1.25, case_length-2*1.25, 2, 2);
};

module case(){
    difference() {
        case_base();
        case_cuts();
    }
};

case();
