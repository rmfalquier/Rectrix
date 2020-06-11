# set parametric
# set isosamples 50,10
# set hidden
# set key below

# set title "Parametric Sphere"
# set urange [-pi/2:pi/2]
# set vrange [0:2*pi]
# set ztics nomirror -1.0,0.25,1.0
# set view 45,50
# splot cos(u)*cos(v),cos(u)*sin(v),sin(u)

splot ".output/cells.rctx" using 1:2:3 w lines