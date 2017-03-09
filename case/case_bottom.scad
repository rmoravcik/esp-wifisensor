include <rounded_cube.scad>
include <dimensions.scad>

module case_base(){
    rounded_cube(case_width,case_length,case_height,3);
    translate([1.25,1.25,case_height]) rounded_cube(case_width-2*1.25, case_length-2*1.25, 2, 2);
};

module case_cuts(){
    translate([wall_thickness+1.5, wall_thickness+1.5,wall_thickness]) cube([pcb_width-3, 31.5, chamber_height+wall_thickness]);
    translate([wall_thickness, wall_thickness+2*1.5+31.5,wall_thickness]) cube([pcb_width, pcb_length-2*1.5-31.5, chamber_height+wall_thickness]);
    translate([wall_thickness, wall_thickness,chamber_height+wall_thickness]) cube([pcb_width, pcb_length, case_height]);

    translate([case_width/2-0.5,14,0]) rounded_cube(1,8,10,0.5);
};

module mounting_base(){
    translate([28.5+wall_thickness+0.2, 11.5+wall_thickness+0.2, wall_thickness]) cylinder(r=6/2, h=chamber_height,$fn=16);
    translate([33+wall_thickness+0.2, 60+wall_thickness+0.5, wall_thickness]) cylinder(r=6/2, h=chamber_height,$fn=16);};

module mounting_cuts(){
    translate([28.5+wall_thickness+0.2, 11.5+wall_thickness+0.2, wall_thickness]) cylinder(r=2/2, h=chamber_height,$fn=16);
};

module hole_base(){
    translate([case_width/2-1.25,13,0]) cube([2.5,12,wall_thickness+2.5]);
};

module hole_cuts(){
    translate([case_width/2-0.5,14,0]) rounded_cube(1,8,wall_thickness+2,0.5);
    translate([case_width/2-0.5,20,wall_thickness]) cube([1,8,2]);
    translate([case_width/2-1.25,23.5,wall_thickness]) cube([2.5,2,2.5]);

};

module case(){
    difference() {
        case_base();
        case_cuts();
    }

    difference() {
        mounting_base();
        mounting_cuts();
    }

    difference() {
        hole_base();
        hole_cuts();
    }
};

case();
