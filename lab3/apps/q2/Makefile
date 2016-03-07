default:
	cd makeprocs; make
	cd injection_hydro; make
	cd injection_sulphate; make
	cd reaction1; make
	cd reaction2; make
	cd reaction3; make

clean:
	cd makeprocs; make clean
	cd injection_hydro; make clean
	cd injection_sulphate; make clean
	cd reaction1; make clean
	cd reaction2; make clean
	cd reaction3; make clean
	

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 4 4 
