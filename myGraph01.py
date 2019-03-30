# coding: shift_jis
#################################################################################
#
# 
#
#################################################################################

import  os
import  sys
import 	json
import 	csv
import  datetime
import 	locale
import  numpy 				as 	np
import 	matplotlib.pyplot 	as 	plt
import  pandas				as 	pd
from 	mpl_toolkits.mplot3d 	import Axes3D           # (A)
#%matplotlib inline

import  myGraphSub			as 	sb1


#################################################################################
#
# MAIN PROGRAM
#
#################################################################################
	
print('------------------------------------------------- MAIN-----' );
args = sys.argv	
argc = len( args )
print('ARGC=' , argc )
#
#-------------------- LIST,MATRIX
#-------------------- GRAPH
#sb1.g0()
#sb1.sample03B()
#sb1.g3D()
#sb1.g3D2()
#sb1.gBase2()
#sample03B()
#sample03C()
#sample03D()
x = np.arange(-3, 3, 0.1)
y = np.sin(x)
#b1.g0a( x ,y )
#
print('------------------------------------------------- sample:03B( 3D-graph )' );
xn       = 9
x0       = np.linspace( -2, 2, xn )            # (A)
x1       = np.linspace( -2, 2, xn )            # (B)

sb1.g3Da( xn, x0,x1 )
#	xn       = 9
#	x0       = np.linspace( -2, 2, xn )            # (A)
#	x1       = np.linspace( -2, 2, xn )            # (B)


