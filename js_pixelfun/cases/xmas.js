snow_alloc = 0;

function setcolor(k, col) {
    pix[k + 0] = col[0];
    pix[k + 1] = col[1];
    pix[k + 2] = col[2];
}
function is_empty(k) {
    return pix[k] == 0 && pix[k+1] == 0 && pix[k+2] == 0;
}

function sheen(self) {
    var x = self.x + self.k, y = self.y - self.k;
    var k  = i_of_xy(x,y);
    var kt = i_of_xy(x - 2*self.k_init, y - 2*self.k_init);
    pix[k+0] = pix[kt+0];
    pix[k+1] = pix[kt+1];
    pix[k+2] = pix[kt+2];

    self.k -= 1;
    if(self.k>0){
	reg(self);
    } else if( self.k2 > 1 ){
	self.x += 1;
	self.k2 -= 1;
	reg(self);
    }
}

function ball(self, params) {
    for(var i = -self.k2 ; i <= self.k2 ; i++ ) {
	var d = Math.abs(i);
	var k = i_of_xy(self.x + i, self.y + self.k2 - d);
	if( self.front || is_empty(k) ){
	    setcolor(k,  params.ball_cols[self.col]);
	}
	var k2 = i_of_xy(self.x + i, self.y - self.k2 + d);
	if( self.front || is_empty(k2) ){
	    setcolor(k2, params.ball_cols[self.col]);
	}
    }
	
    self.k2 = self.k2 - 1;
    if(self.k2 > 0 ){ reg(self,20); }
    else {
	var k = Math.floor(0.8*self.k3);
	reg({fun:sheen, x:self.x - k, y:self.y, k:k, k2:k, k_init:k});
    }
}

function ball_place(self, params) {
    snow_alloc += 2;
    self.y += 1;
    self.k -= 1;
    setcolor(i_of_xy(self.x,self.y), [120,120,0]);
    if(self.k <= 0) { self.fun = ball; self.front = (self.k3%2 == 0); }
    reg(self);
}

function leaf(self, params) {
    snow_alloc += 0.5;
    self.x += self.dx; self.y += self.dy;
    if(self.y > h-2){ return; }

    var k = i_of_xy(self.x,self.y);
    if( !self.not_first || is_empty(k) ) {
	self.not_first = true;
	setcolor(k, params.leaf_color);

	self.k -= 1;
	if(self.k > 0) { reg(self); }
	else{
	    self.k2 = Math.floor(self.k2/2);
	    if( self.k2 > 0 ) {
		self.dx = Math.random() > 0.5 ? +1 : -1;
		self.k = self.k2;
		reg(self, params.leaf_wait)
		if( Math.random() < params.p_split ) {
		    reg({fun:leaf, x:self.x,y:self.y, dx:-self.dx, dy:self.dy,
			 k:self.k, k2:self.k2}, params.leaf_wait)
		}
	    }
	}
    } else {  // Hit something.
	var k3 = self.k2/1.5 + self.k2%4, col = Math.floor(self.k)%params.ball_cols.length;
	reg({fun:ball_place, x:self.x, y:self.y,
	     k:2*self.k, k2:k3, k3:k3, col:col}, 10)
    }
}

function branch(self, params) {
    snow_alloc += 2;    
    self.x += self.dx;

    if( Math.random() < params.p_leaf ) {
	reg({fun:leaf, x:self.x, y:self.y, k:5+10*Math.random(), k2:15+5*Math.random(),
	     dx:0,dy:1});
    }

    var we = 0.01, wl = 1/self.k/self.k;
    if( Math.random() < (we*params.p_rand_down + wl*params.p_rand_down_late)/(we+wl) ){
	self.y += 1;
    }
    setcolor(i_of_xy(self.x,self.y), params.branch_color);

    self.k = self.k - 1;
    if(self.k > 0) { reg(self); }
}
function stem(self, params) {
    snow_alloc += 1;
    if( self.k < params.early_k && Math.random() < params.p_branch ) {
	var dx = Math.random() < self.pd ? -1 : +1;
	self.pd += dx * params.p_dir_drift;
	reg({fun:branch, k:self.k, x:self.x, y:self.y, dx:dx});
    }
    var k = i_of_xy(self.x,self.y);
    for(var i = -(Math.floor(self.k/100)); i <= (self.k+10)/100 ; i++) {
	setcolor(4*i + k, params.stem_color);
    }
    self.k -= 1;
    self.y -= 1;
    if( self.k > 0 ) { reg(self); }
}

function snowdrift(self) {
    var dx = Math.random() > 0.5 ? + 1 : -1;
    var k = i_of_xy(self.x,self.y);

    if( is_empty(k+4*w) ){ self.y +=1; reg(self); }
    else if(is_empty(k+4*( dx+w)) && is_empty(k+4*dx)){ self.x+=dx; self.y+=1; reg(self); }
    else if(is_empty(k+4*(-dx+w)) && is_empty(k-4*dx)){ self.x-=dx; self.y+=1; reg(self); }
    else {
        setcolor(k, [255,255,255]);
    }
}
function snow(self) {
    if( snow_alloc <= 0 ){ reg(self); return; }

    self.k--;
    var y=0, x = Math.floor(Math.random()*w);
    while( y < h-1 ){
	y += 1;
	var k = i_of_xy(x,y);
	if( pix[k] != 0 || pix[k+1]!=0 || pix[k+2] != 0) {
	    snow_alloc -= 1;
	    reg({fun:snowdrift, x:x,y:y-1});
	    break;
	}
    }
    if( self.k>0 ){ reg(self); }
}

named_patterns.xmas = {
    params : {
	p_branch : 0.1,
	p_rand_down:0.5, p_rand_down_late:0,
	p_leaf : 0.1, p_split:0.6,

	p_dir_drift:0.3,
        init_k : Math.floor(0.8*w), early_k : 0.6*w,

	branch_color : [0,180,0],
	stem_color : [80,40,40],
	leaf_color : [0,220,0],
	ball_cols : [[255,0,0], [255,255,0], [0,0,255]],
	leaf_wait : 20
    },
    setup : function(params) {
	for(var k = 0 ; k < 4*w*h ; k++ ){ pix[k] = (k%4 == 3 ? 255 : 0); }
	reg({fun:stem, x:w/2, y:Math.floor(0.9*h), k:params.init_k, pd:0.5});
	for(var n = 0 ; n<100 ; n++){
	    reg({fun:snow, k:450});
	}
    }
}
