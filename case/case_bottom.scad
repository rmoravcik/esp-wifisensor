dps_sirka = 40+0.2;
dps_dlzka = 77+0.2;
dps_vyska = 1.6;
zapustenie_dps = 6;
sirka_steny = 2.5;
sensor_vyska = dps_vyska+zapustenie_dps+21.4;

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
    translate([1.25,1.25,krabicka_vyska]) rounded_cube(krabicka_sirka-2*1.25, krabicka_dlzka-2*1.25, 2, 2);
};

module krabicka_cuts(){
    translate([sirka_steny+1.5, sirka_steny+1.5,sirka_steny]) cube([dps_sirka-3, 31.5, zapustenie_dps+sirka_steny]);
    translate([sirka_steny, sirka_steny+2*1.5+31.5,sirka_steny]) cube([dps_sirka, dps_dlzka-2*1.5-31.5, zapustenie_dps+sirka_steny]);
    translate([sirka_steny, sirka_steny,zapustenie_dps+sirka_steny]) cube([dps_sirka, dps_dlzka, krabicka_vyska]);

    translate([krabicka_sirka/2-0.5,14,0]) rounded_cube(1,8,10,0.5);
};

module distanc_base(){
    translate([28.5+sirka_steny+0.1, 11.5+sirka_steny+0.2, sirka_steny]) cylinder(r=6/2, h=zapustenie_dps,$fn=16);
    translate([33+sirka_steny+0.1, 60+sirka_steny+0.5, sirka_steny]) cylinder(r=6/2, h=zapustenie_dps,$fn=16);};

module distanc_cuts(){
    translate([28.5+sirka_steny+0.1, 11.5+sirka_steny+0.1, sirka_steny]) cylinder(r=2/2, h=zapustenie_dps,$fn=16);
};

module otvor_base(){
    translate([krabicka_sirka/2-1.25,13,0]) cube([2.5,12,sirka_steny+2.5]);
};

module otvor_cuts(){
    translate([krabicka_sirka/2-0.5,14,0]) rounded_cube(1,8,sirka_steny+2,0.5);
    translate([krabicka_sirka/2-0.5,20,sirka_steny]) cube([1,8,2]);
    translate([krabicka_sirka/2-1.25,23.5,sirka_steny]) cube([2.5,2,2.5]);

};

module krabicka(){
    difference() {
        krabicka_base();
        krabicka_cuts();
    }

    difference() {
        distanc_base();
        distanc_cuts();
    }

    difference() {
        otvor_base();
        otvor_cuts();
    }
};

krabicka();
