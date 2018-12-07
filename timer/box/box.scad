
translate([0,0,90])
rotate([0,90,0]) {
difference() {
union() {
difference() {
    color([1,1,0])
    cube([90,90,80]);
    
    color([1,0,0])
    translate([-5,5,5])
    cube([90,80,70]);
    
    color([0,1,0])
    translate([30,8,70])
    cube([50,70,20]);
}

color([0,0,1])
translate([12.5,2.5,45])
cube([75,85,5]);

color([1,0.5,0])
translate([0,2.5,5])
cube([10,85,5]);

color([0.5,0.5,0])
translate([0,2.5,70])
cube([10,8,5]);

color([0.5,0.5,0])
translate([0,78,70])
cube([10,10,5]);
}


color([1,0,1])
translate([5,0,75])
rotate([0,90,90])
linear_extrude(height=85)
polygon([[0,0],[5,5],[65,5],[70,0]]);


color([1,0,1])
translate([10,5,75])
rotate([0,90,90])
linear_extrude(height=80)
polygon([[0,0],[5,5],[65,5],[70,0]]);
}

}


color([1,0,1])
rotate([90,0,0])
translate([5,0,10])
linear_extrude(height=85)
polygon([[0,0],[5,5],[65,5],[70,0]]);

