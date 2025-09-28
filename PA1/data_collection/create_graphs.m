clear; close all; clc;

data_table = sortrows(readtable("pa1_data_2025-09-26_11-08-32.csv"),{'type','target','threads'},"ascend");

types = unique(data_table.type);
targets = unique(data_table.target);
threads = unique(data_table.threads);

separated_tables = double(zeros(length(types),length(targets),length(threads),100));


for i = 1:height(data_table)
    
    separated_tables( ...
        contains(types, data_table(i,"type").type), ...
        find(targets, data_table(i,"target").target), ...
        find(threads, data_table(i,"threads").threads), ...
        (mod(i,100)+1)) = data_table(i,"time").time;
end


for type=1:length(types)
    for target=1:length(targets)
        for thread=1:length(threads)
            x(:) = separated_tables(type,target,thread,:);
            m = mean(x);
            s = std(x);
            if(m~=0)
                display("Data for: " + types(type) + " " + num2str(targets(target)) + " " + num2str(threads(thread)) + " == "...
                    + "Mean: " + num2str(mean(x)) + " STD: " + num2str(std(x)))
            end
            
        end
    end
end