pvs-studio-analyzer trace -- make &&
pvs-studio-analyzer analyze -i -o pvs.log &&
plog-converter -a GA:1,2 -t tasklist -o pvs.tasks pvs.log &&
plog-converter -t html pvs.tasks > pvs.html
