height = [];
water = [];
for( var i = 0 ; i < w*h ; i++ ){ height.push(0);water.push(0); }
function hk(x,y){ return (x + w*y)%(w*h); }
function rand_hk(){ return hk(rand_x(), rand_y()); }

function en2_sink_land(k, kt) {
    var s = 1;
    var d = Math.floor((height[k] - height[kt] - s)/2);
    if( d>0 ) {
        var w = water[k] - height[k];
        height[k]  -= d;
        water[k] = height[k] + w;
        var wt = water[kt] - height[kt];
        height[kt] += d;
        water[kt] = height[kt] + wt;
    }
}


function en2_sink_water_d(k,kt) {
    var s = 0;
    var d = Math.floor((water[k] - water[kt] -s)/2);
    return Math.min(d, water[k]-height[k]);
}
function en2_sink_water(k,kt) {
    var d = en2_sink_water_d(k,kt);
    if( d>0 ) {
        water[k]  -= d;
        water[kt] += d;
    }
}

function en2_sink(k,kt) {
//    if(Math.random()<0.1){
    en2_sink_land(k,kt);
    var d = Math.floor(en2_sink_water_d(k,kt)/2);
    if( d > 0 ){
        water[k]  -= d;
        reg({fun:en2_raindrop, k:kt, route:[], w:d});
    }
    //en2_sink_water(k,kt);
}

// Won't go, because it has height..
function en2_raindrop(self, params) {
    var wh = w*h, k = self.k;
    var dks = [w, 1, wh-w, wh-1];
    var i=Math.floor(8+8*Math.random()), di=(Math.random()>0.5 ? -1 : +1);
    var max_k=-1, max_d = -1;
    for( var m=0; m<4 ; m++ ){
        var kt = (k + dks[i%4])%wh;
        var d = en2_sink_water_d(k, kt);
        if( d > max_d ) {
            max_d = d;
            max_k = kt;
        }
        i += di;
    }
    if(max_d > 0 ) {
        self.route.push(k);
        self.k = max_k;
        reg(self);
    } else {  // End of line.
//        for( var m=0 ; m<3 ; m++ ){ self.route.pop(); }

        for( var j in self.route) {
            var k = self.route[j];
            var c = Math.floor(
                5*Math.sqrt(self.w)/(1 + 8*Math.pow(water[k]-height[k],2)));
            height[k] -= c;
            water[k] -= c;
        }
        water[self.k]+= self.w;
    }
}

var water_min=0, water_max=0, height_min=0, height_max=0;

function en2_ts(self, params) {

    //for( var q=0 ; q<1 ; q++ ){
    if( Math.random() < 0.01 ) {
        reg({fun:en2_raindrop, k:rand_hk(), route:[], w:2});
//        water[hk(rand_x(), rand_y())] += 200;
    }
    for( var q=0 ; q<3 ; q++ ){
        if(height[k] < water[k] ){ water[k]--; }
    }
/*    for( var q=0 ; q<60 ; q++ ){
        var k = hk(rand_x(), rand_y());
        if(height[k] < water[k]-10 ){ water[k] -= 1; height[k]--; }
    }*/

    var wh = w*h;
    for( var k = self.k ; k < self.k+20*w ; k += 3 ) {
        var r = Math.random();
        if(r<0.5) {
            if(r<0.25) { en2_sink(k, (k+w)%wh); }
            else {       en2_sink(k, (wh+k-w)%wh); }
        } else {
           if(r>0.75) { en2_sink(k, (k+1)%wh); }
            else {       en2_sink(k, (wh+k-1)%wh); }
        }
    }
    self.k += 20*w;
    if(self.k >= wh) {
        self.k_off = (self.k_off + 1)%3;
        self.k = self.k_off;
        self.k_next_erode -= wh;
    }

    if(self.n %8 == 0) {
//        if(self.n%(8*600) == 0 ) { en2_raise(w*w/16, 200); }

        height_min = 0; height_max = 0;
        for( var k = 0 ; k<wh ; k++ ) {
            if(height[k] < height_min) { height_min = height[k]; }
            if(height[k] > height_max) { height_max = height[k]; }
            if(water[k] < water_min) { water_min = water[k]; }
            if(water[k] > water_max) { water_max = water[k]; }
        }
  
        for( var k = 0 ; k<wh ; k++ ) {
            pix[4*k+0] = Math.floor(255*(height[k] - height_min)/(height_max-height_min));
            //pix[4*k+1] = 128 + 10*(height[k] - height[(k+w)%(w*h)]);
            pix[4*k+1] = 128;
            //pix[4*k+2] = 128 - (water[k] - water[(k+w)%(w*h)]);
            //pix[4*k+2] = Math.floor((water[k] - water_min)/(water_max-water_min));
            if( height[k] > water[k]+1 ) {
                pix[4*k+2] = 128;
            } else {
//                pix[4*k+0] /= 1 + (water[k]-height[k])/40;
                pix[4*k+1] /= 1 + (water[k]-height[k])/40;
                pix[4*k+2] =
                    Math.min(255,Math.floor(10*(water[k]-height[k]))) ;
            }
        }
    }
    self.n += 1;
    reg(self, 2);


    for(var k = Math.floor(0.47*w) ; k<0.5*w ; k++){
        water[w*h*0.8 + k] += 50;
    }
    for(var k = 0 ; k<w ; k++ ){
        height[k] = 0;
        water[k] = 8000;
        height[wh-k-1] = 2*Math.floor(256*h);
        water[wh-k-1] = height[wh-k-1];
    }
/*    var k = hk(0.4*w,0.75*h);
      water[k] = height[k] + 256*20;

    var k = hk(0.2*w,0.86*h);
    water[k] = height[k] + 256*20; */
}

function en2_raise(x,y, r,q) {
    for( var i = 0 ; i<w ; i++ ) {
        for( var j = 0 ; j<h ; j++ ) {
            var k = i+w*j;
            var dsqr = (i-x)*(i-w/2) +(j-y)*(j-h/2);
            if( dsqr < r*r ){ 
                height[k] += q;
                water[k] += q;
            }
        }
    }
}

named_patterns.n_erosion2 = {
    params : { ks : 300, dirs:[[0,1],[1,0], [0,-1],[-1,0]] },
    setup : function(params) {
/*        for( var i = 0 ; i<w ; i++ ) {
            for( var j = 0 ; j<h ; j++ ) {
                var k = i+w*j;
                height[k] += Math.floor(2*Math.random());

                water[k] = 256*height[k]; //256*height[k];// + (j>w/2 ? 1 : 0);
            }
        }
        en2_raise(w/2, h/2, w/4, 256);

        en2_raise(0,0, 0.3*w, 100);
        en2_raise(0.04*w,0, 0.04*w, -10);
*/
        for( var i = 0 ; i<w ; i++ ) {
            for( var j = 0 ; j<h ; j++ ) {
                var k = i+w*j;
                height[k] = 256*j;// + Math.floor(2*Math.random());
                water[k] = height[k]; //256*height[k];// + (j>w/2 ? 1 : 0);
            }
        }
        for( var m=0; m< 100 ; m++ ){
            var k = rand_hk();
            height[k] += 160*256*h;
            water[k] = height[k];
        }

        reg({fun:en2_ts, n:0, k_off:0, k:0});
    }
}
