default:
	cd userprog; make
	cd test_dynamic; make

clean:
	cd userprog; make clean
	cd test_dynamic; make clean
	
run:
	cd ../../bin; dlxsim -x os.dlx.obj -a  -u userprog4.dlx.obj 2; ee469_fixterminal
