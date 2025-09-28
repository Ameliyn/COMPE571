clear; close all force; clc;

data_table = sortrows(readtable("pa1_data_2025-09-26_11-08-32.csv"),{'type','target','threads'},"ascend");

types = string(unique(data_table.type));
targets = string(unique(data_table.target));
threads = string(unique(data_table.threads));

separated_tables = double(zeros(length(types),length(targets),length(threads),100));


for i = 1:height(data_table)
    
    separated_tables( ...
        strcmp(types, string(data_table(i,"type").type)), ...
        strcmp(targets, string(data_table(i,"target").target)), ...
        strcmp(threads, string(data_table(i,"threads").threads)), ...
        (mod(i,100)+1)) = data_table(i,"time").time;
end

separated_results = string(zeros(length(types),length(targets),length(threads),3));
names = {};
means = {};
stds = {};

for target=1:length(targets)
    names{target} = [];
    means{target} = [];
    stds{target} = [];
    for thread=1:length(threads)

        for type=1:length(types)
            x(:) = separated_tables(type,target,thread,:);
            
            if(mean(x)~=0)
                names{target} = [names{target}, "" + types(type) + " (" + threads(thread) + ")"];
                means{target} = [means{target}, mean(x)];
                stds{target} = [stds{target}, std(x)];
                separated_results(type, target, thread, :) = [names{target}(end), means{target}(end), stds{target}(end)];
            end
        end
    end
end


for target=1:length(targets)
    T = table(names{target}(:), ...
        means{target}(:), ...
        stds{target}(:));
    f = uifigure('Position', [100 100 400 400]);
    uitable(f, 'Data', T, ...
        'ColumnName', {"Name (Threads)", "Mean", "Standard Deviation"}, ...
        'RowName', T.Properties.RowNames, ...
        'Units', 'Normalized', ...
        'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
    f.Name = "Target: " + sprintf("%.1e", str2num(targets(target)));
end