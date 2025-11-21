include <BOSL2/std.scad>
include <BOSL2/hinges.scad>

$fn = 25;

X = 60;
Y = 90;
Z = 3;
hingeGap = 0.5;

module pad(padX=X, padY=Y, padZ=Z) {
cube([padX,padY, padZ]);
}

// hinge support
module hinge(xRot=180, yRot=180, L=X) {
    knuckle_hinge(length=L,segs=10, offset=2, in_place=true);
    xrot(xRot, cp=[0,0,0])
    yrot(yRot, cp=[0,0,0])
    knuckle_hinge(length=L,segs=10, offset=2, in_place=true, inner=true);

}
//close pad
pad();
translate([X*0.5, Y + hingeGap, Z])
hinge();

//far pad
translate([0, Y + (2 * hingeGap)])
pad();

// button thing
bOff = 10;
/*
translate([X*0.5 + bOff, Y*0.33, Z+1.5])
difference() {
cube([4,6,6], center=true);
cylinder(h=100, r=.5);
}

translate([X*0.5 - bOff, Y*0.33, Z+1.5])
difference() {
cube([4,6,6], center=true);
cylinder(h=100, r=.5);
}*/

// button "base"
translate([X*0.5, Y*0.33, Z+1.5])
cube([13,13,5], center=true);

// cable run
translate([X*0.15, Y*0.33, Z+1])
cube([2,2,4], center=true);

//top
translate([X*0.15, Y*0.305, Z+2.5])
cube([1,6,1], center=true);

translate([X*0.15, Y*0.28, Z+1])
cube([2,2,4], center=true);

//spring thing
translate([X*0.5, Y*0.13, Z+1.5])
cube([4,4,4], center=true);
translate([X*0.5, Y+Y*0.87, Z+1.5])
cube([4,4,4], center=true);


// top button acceptor
translate([X*0.5, Y+Y*0.66, Z])
cube([2*bOff,8,2], center=true);



