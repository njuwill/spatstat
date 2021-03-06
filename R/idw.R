#
#  idw.R
#
#  Inverse-distance weighted smoothing
#
#  $Revision: 1.9 $ $Date: 2017/06/05 10:31:58 $

idw <- function(X, power=2, at="pixels", ...) {
  stopifnot(is.ppp(X) && is.marked(X))
  marx <- marks(X)
  if(is.data.frame(marx)) {
    if(ncol(marx) > 1) {
      # multiple columns of marks - process one-by-one
      out <- list()
      for(j in 1:ncol(marx)) 
        out[[j]] <- idw(X %mark% marx[,j], power=power, at=at, ...)
      names(out) <- names(marx)
      switch(at,
             pixels = { out <- as.solist(out) },
             points = { out <- as.data.frame(out) })
      return(out)
    } else 
      marx <- marx[,1L]
  }
  if(!is.numeric(marx))
    stop("Marks must be numeric")
  check.1.real(power)
  switch(at,
         pixels = {
           # create grid
           W <- as.mask(as.owin(X), ...)
           dim <- W$dim
           npixels <- prod(dim)
           # call C
           z <- .C("Cidw",
                   x = as.double(X$x),
                   y = as.double(X$y),
                   v = as.double(marx),
                   n = as.integer(npoints(X)),
                   xstart = as.double(W$xcol[1L]),
                   xstep  = as.double(W$xstep),
                   nx     = as.integer(dim[2L]),
                   ystart = as.double(W$yrow[1L]),
                   ystep  = as.double(W$ystep),
                   ny     = as.integer(dim[1L]),
                   power  = as.double(power),
                   num    = as.double(numeric(npixels)),
                   den    = as.double(numeric(npixels)),
                   rat    = as.double(numeric(npixels)),
                   PACKAGE = "spatstat")
           out <- as.im(matrix(z$rat, dim[1L], dim[2L]), W=W)
           out <- out[W, drop=FALSE]
         },
         points={
           npts <- npoints(X)
           z <- .C("idwloo",
                   x = as.double(X$x),
                   y = as.double(X$y),
                   v = as.double(marx),
                   n = as.integer(npts),
                   power  = as.double(power),
                   num    = as.double(numeric(npts)),
                   den    = as.double(numeric(npts)),
                   rat    = as.double(numeric(npts)),
                   PACKAGE = "spatstat")
           out <- z$rat
         })
  return(out)
}
