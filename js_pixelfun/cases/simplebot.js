// Author: Jasper den Ouden, 10-2017

// Simple "evolving" bots that choose to go right or left and eat where they go.
// self.sel selects what parameter to evolve.

function simplebot(self, params) {
    var dx = null, dy = null;
    switch( self.dir%4 ) {
    case 0: dx = 0;  dy = 1; break;
    case 1: dx = 1;  dy = 0; break;
    case 2: dx = 0;  dy = -1; break;
    case 3: dx = -1; dy = 0; break;
    }
//    alert(self.dir + ":" + dx + " " + dy);

    var x = self.x, y = self.y; // "eyes"
    var fi = i_of_xy(x + dx, y + dy); // Forward
    var ri = i_of_xy(x + dx + dy, y + dy + dx); // right(or left, whatevs)
    var li = i_of_xy(x + dx - dy, y + dy - dx); // ..

    var fv = (pix[fi+1] > 127 ? 1 : 0); // 0/1 values of three eyes.
    var rv = (pix[ri+1] > 127 ? 1 : 0);
    var lv = (pix[li+1] > 127 ? 1 : 0);
    self.sel += fv + 2*rv * 4*lv;
    var arr = self.arr;
    var choose = 1 + arr[0]*fv + arr[1]*rv + arr[2]*lv
        + arr[3]*fv*rv + arr[4]*rv*lv + arr[5]*fv*lv
        + arr[6]*fv*rv*lv;
    self.dir += 4 + ( (choose%2 == 1) ? 1 : -1 ); // Right/left.
    self.dir = self.dir % 4;

    self.x += dx; self.y += dy;
    if( fv == 1 ) { // Eat one.
        pix[fi] = 0; pix[fi+1] = 0; pix[fi+2] = 0;
        self.food += params.bite;
        if( self.food > params.reprod ){  // Reproduce.
            self.food -= params.reprod;
            var new_arr = [];  // Copy array, pick one to change.
            for( var k in arr){ new_arr[k] = arr[k]; }
            new_arr[self.sel%7] += 1;
            pix[fi+2] = 255;
            self.food -= params.pass_food;
            reg({fun:simplebot, x:x, y:y, dir:(self.dir + 2),
                 age:0, food: params.pass_food,
                 arr : new_arr, sel:0
                });
        }
        self.age += params.age_feed;
    } else {
        self.food -= params.hunger;
        self.age += params.age_starve;
    }
    if( self.age > params.max_age || self.food < 0 ){
        pix[fi] = 255;  // Die, leave mark.
    } else {
        reg(self);  // Still alive.
    }
}

named_patterns.simplebot = {
    params : {
        init_x : w/2, init_y : h/2,
        max_age :120,
        reprod : 10,
        pass_food : 0, init_food : 0,
        bite :1, age_starve:1, age_feed:1, hunger:0,
    },
    setup : function(params) {
        reg({fun:simplebot, x:params.init_x, y:params.init_y, dir:0,
             age:0, food:params.init_food,
             arr : [0,1,1,1,1,1,0], sel:0,
            });
    }
}
