function dialen_diag(self) {
    var x = self.x, y = self.y, i = 0;
    while( i < self.ks ) {
        i++;
        x += self.dx; y += self.dy;
        var k1 = i_of_xy(x,y);
        if( pix[k] > 120 ){  // Empty, just color.
            pix[k+1] = 0;
        } else { //spawn more with some offset
            self.ks -= 6;
            if(self.ks < 0){ return; }
        }
    }
    // Make it/
    reg({fun:dialen_diag, x:x, y:y + 5, dx : self.dx, dy:-self.dy,
         ks : self.ks + 2});
    reg({fun:dialen_diag, x:x, y:y - 4, dx :-self.dx, dy:self.dy,
         ks : self.ks + 2});
}

named_patterns.dialen = {
    params : { ks : 6 },
    setup : function(params) {
        reg({fun:dialen_diag, x:w/2, y:h/2, dx:1, dy:1, ks : params.ks});
    }
}
