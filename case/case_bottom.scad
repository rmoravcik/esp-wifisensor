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

    translate([(case_width-(case_width/1.618))/2,11,0]) rounded_cube(case_width/1.618,2,2,0.5);
    translate([(case_width-(case_width/1.618))/2,16,0]) rounded_cube(case_width/1.618,2,2,0.5);
    translate([(case_width-(case_width/1.618))/2,21,0]) rounded_cube(case_width/1.618,2,2,0.5);

    translate([(case_width-8)/2,13,1]) cube([8,2,2]);
    translate([(case_width-8)/2,18,1]) cube([8,2,2]);
    translate([(case_width-8)/2,23,1]) cube([8,2,2]);
};

module mounting_base(){
    translate([28.8+wall_thickness, 11.6+wall_thickness, wall_thickness]) cylinder(r=6/2, h=chamber_height,$fn=16);
    translate([33+wall_thickness+0.2, 60+wall_thickness+0.5, wall_thickness]) cylinder(r=6/2, h=chamber_height,$fn=16);};

module mounting_cuts(){
    translate([28.8+wall_thickness, 11.6+wall_thickness, wall_thickness]) cylinder(r=2/2, h=chamber_height,$fn=16);
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
};

case();
