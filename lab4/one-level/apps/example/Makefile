default:
	cd makeprocs; make
	cd hello_world; make
	cd part2; make
	cd spawn_counting; make

clean:
	cd makeprocs; make clean
	cd hello_world; make clean
	cd part2; make clean
	cd spawn_counting; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 5; ee469_fixterminal

runm:
	cd ../../bin; dlxsim -D m -x os.dlx.obj -a -D m -u makeprocs.dlx.obj 5; ee469_fixterminal

runmp:
	cd ../../bin; dlxsim -D mp -x os.dlx.obj -a -D mp -u makeprocs.dlx.obj 5; ee469_fixterminal

runa:
	cd ../../bin; dlxsim -D a -x os.dlx.obj -a -D a -u makeprocs.dlx.obj 5; ee469_fixterminal
