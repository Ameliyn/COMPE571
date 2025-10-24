clear; close all force; clc;

data_table = sortrows(readtable("pa2_data_2025-10-21_22-56-07.csv"),{'Type','Quantum'},"ascend");

types = string(unique(data_table.Type));
quantums = string(unique(data_table.Quantum));
avg_response_times = string(unique(data_table.AverageResponseTime));
context_switch_times = string(unique(data_table.ContextSwitchTime));

avg_response_times = double(zeros(length(types),length(quantums),100));
context_switch_times = double(zeros(length(types),length(quantums),100));


for i = 1:height(data_table)
    
    avg_response_times( ...
        strcmp(types, string(data_table(i,"Type").Type)), ...
        strcmp(quantums, string(data_table(i,"Quantum").Quantum)), ...
        (mod(i,100)+1)) = data_table(i,"AverageResponseTime").AverageResponseTime;
    context_switch_times( ...
        strcmp(types, string(data_table(i,"Type").Type)), ...
        strcmp(quantums, string(data_table(i,"Quantum").Quantum)), ...
        (mod(i,100)+1)) = data_table(i,"ContextSwitchTime").ContextSwitchTime;
end

separated_results = string(zeros(length(types),length(quantums),3));
names = {};
means = {};
stds = {};

for quantum=1:length(quantums)
% for quantum=1:2
    names{quantum} = [];
    means{quantum} = [];
    stds{quantum} = [];
    for type=1:length(types)
        x(:) = avg_response_times(type,quantum,:);
        
        if(mean(x)~=0)
            names{quantum} = [names{quantum}, quantums(quantum)];
            means{quantum} = [means{quantum}, mean(x)];
            stds{quantum} = [stds{quantum}, std(x)];
            separated_results(type, quantum, :) = [names{quantum}(end), means{quantum}(end), stds{quantum}(end)];
        end
    end
end

round_robin(:,:) = reshape(separated_results(strcmp(types, "rr"),:,:), 51,3);
multi_level_feedback_queue = reshape(separated_results(strcmp(types, "mlfq"),:,:),51,3);

T = table(multi_level_feedback_queue(:,1), multi_level_feedback_queue(:,2), multi_level_feedback_queue(:,3));
f = uifigure('Position', [100 100 400 368]);
uitable(f, 'Data', T, ...
    'ColumnName', {"Case (Quantum)", "Mean", "Standard Deviation"}, ...
    'RowName', T.Properties.RowNames, ...
    'Units', 'Normalized', ...
    'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
f.Name = "MLFQ";

T = table(round_robin(:,1), round_robin(:,2), round_robin(:,3));
f = uifigure('Position', [100 100 400 368]);
uitable(f, 'Data', T, ...
    'ColumnName', {"Case (Quantum)", "Mean", "Standard Deviation"}, ...
    'RowName', T.Properties.RowNames, ...
    'Units', 'Normalized', ...
    'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
f.Name = "Round Robin";

f = figure();
ax = axes(f);
plot(ax,str2double(round_robin(:,1)),str2double(round_robin(:,2)));
f.Name = "Round Robin";

f2 = figure();
ax2 = axes(f2);
plot(ax2,str2double(multi_level_feedback_queue(:,1)),str2double(multi_level_feedback_queue(:,2)));
f2.Name = "Mutli Level Feedback Queues";

% for quantum=1:length(quantums)
% for quantum=1:2
%     T = table(names{quantum}(:), ...
%         means{quantum}(:), ...
%         stds{quantum}(:));
%     f = uifigure('Position', [100 100 400 368]);
%     uitable(f, 'Data', T, ...
%         'ColumnName', {"Case (Quantum)", "Mean", "Standard Deviation"}, ...
%         'RowName', T.Properties.RowNames, ...
%         'Units', 'Normalized', ...
%         'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
%     f.Name = "Quantum: " + quantums(quantum);
%     % exportapp(f, "Tables_" + quantums(quantum) + ".png");
% end