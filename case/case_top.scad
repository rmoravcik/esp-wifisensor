dps_sirka = 40+0.2;
dps_dlzka = 77+0.2;
dps_vyska = 1.6;
sensor_vyska = 29;
zapustenie_dps = 8;
sirka_steny = 2.5;

krabicka_sirka = dps_sirka+2*sirka_steny;
krabicka_dlzka = dps_dlzka+2*sirka_steny;
krabicka_vyska = sensor_vyska/2+sirka_steny;

module rounded_cube(a,b,c,r){
    hull(){
        translate([r,r,0]) cylinder(r=r, h=c, $fn=64);
        translate([a-r,r,0]) cylinder(r=r, h=c, $fn=64);
        translate([a-r,b-r,0]) cylinder(r=r, h=c, $fn=64);
        translate([r,b-r,0]) cylinder(r=r, h=c, $fn=64);
    }
}

module krabicka_base(){
    rounded_cube(krabicka_sirka,krabicka_dlzka,krabicka_vyska,3);
    translate([0,0,krabicka_vyska]) rounded_cube(krabicka_sirka, krabicka_dlzka, 2, 3);
};

module krabicka_cuts(){
    translate([sirka_steny, sirka_steny,sirka_steny]) cube([dps_sirka, dps_dlzka, krabicka_vyska]);
    translate([1.25,1.25,krabicka_vyska]) rounded_cube(krabicka_sirka-2*1.25, krabicka_dlzka-2*1.25, 2, 2);
};

module krabicka(){
    difference() {
        krabicka_base();
        krabicka_cuts();
    }
};

krabicka();
