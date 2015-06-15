fname = '/home/buq2/src/audible_altimeter_software/build/log_viewer/jump6.txt';
fid = fopen(fname);
rawdata = textscan(fid,'%s\t%f\t%f\n');
fclose(fid);


fields = rawdata{1};
ys = rawdata{3};
xs = rawdata{2};

fieldnames = unique(fields);

data = [];
for ii = 1:numel(fieldnames)
    data.(fieldnames{ii}) = basedata;
    okidx = strcmp(fields,fieldnames{ii});
    data.(fieldnames{ii}).x = xs(okidx);
    data.(fieldnames{ii}).y = ys(okidx);
end

plot(data.altitude_1.x,data.altitude_1.y,'rx-')
%plot(data.altitude_rate.x,data.altitude_rate.y,'rx-')