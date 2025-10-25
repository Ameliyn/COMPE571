clear; close all force; clc;
% data_table = readtable("pa2_data_2025-10-21_22-56-07.csv");

% data_table = readtable("pa2_data_2025-10-23_19-05-33.csv");
data_table = readtable("pa2_data_2025-10-24_12-07-51.csv");

data_table = sortrows(data_table,{'Type','Quantum'},"ascend");

types = string(unique(data_table.Type));
quantums = string(unique(data_table.Quantum));

avg_response_times = double(zeros(length(types),length(quantums),100));
context_switch_times = double(zeros(length(types),length(quantums),100));
queue_usage_percent = double(zeros(length(types),length(quantums),100));

for i = 1:height(data_table)
    
    avg_response_times( ...
        strcmp(types, string(data_table(i,"Type").Type)), ...
        strcmp(quantums, string(data_table(i,"Quantum").Quantum)), ...
        (mod(i,100)+1)) = data_table(i,"AverageResponseTime").AverageResponseTime;
    context_switch_times( ...
        strcmp(types, string(data_table(i,"Type").Type)), ...
        strcmp(quantums, string(data_table(i,"Quantum").Quantum)), ...
        (mod(i,100)+1)) = data_table(i,"ContextSwitchTime").ContextSwitchTime;
    queue_usage_percent( ...
        strcmp(types, string(data_table(i,"Type").Type)), ...
        strcmp(quantums, string(data_table(i,"Quantum").Quantum)), ...
        (mod(i,100)+1)) = data_table(i,"QueueUsage").QueueUsage;
end

separated_results = string(zeros(length(types),length(quantums),5));
names = {};
means = {};
stds = {};
cs_means = {};
queue_usage = {};

for quantum=1:length(quantums)
    names{quantum} = [];
    means{quantum} = [];
    stds{quantum} = [];
    cs_means{quantum} = [];
    queue_usage{quantum} = [];

    for type=1:length(types)
        x(:) = avg_response_times(type,quantum,:);
        y(:) = context_switch_times(type,quantum,:);
        z(:) = queue_usage_percent(type,quantum,:);
        if(mean(x)~=0)
            names{quantum} = [names{quantum}, types(type)];
            names{quantum} = [names{quantum}, quantums(quantum)];
            means{quantum} = [means{quantum}, mean(x)];
            stds{quantum} = [stds{quantum}, std(x)];
            cs_means{quantum} = [cs_means{quantum}, mean(y)];
            queue_usage{quantum} = [queue_usage{quantum}, mean(z)];
            separated_results(type, quantum, :) = [names{quantum}(end), means{quantum}(end), stds{quantum}(end), ...
                cs_means{quantum}(end), queue_usage{quantum}(end)];
        end
    end
end

names = [];
means = [];
stds = [];
cs_means = [];
queue_usage = [];
for quantum=1:length(quantums)
for type=1:length(types)
    x(:) = avg_response_times(type,quantum,:);
    y(:) = context_switch_times(type,quantum,:);
    z(:) = queue_usage_percent(type,quantum,:);
    if (mean(x)~=0)
    names = [names, types(type)];
    % names = [names, quantums(quantum)];
    means = [means, mean(x)];
    stds = [stds, std(x)];
    cs_means = [cs_means, mean(y)];
    queue_usage = [queue_usage, mean(z)];
    separated_results(type, quantum, :) = [names(end), means(end), stds(end), ...
        cs_means(end), queue_usage(end)];
    end
end
end
% [type_len, entry_len, data_len] = size(separated_results(:,:,:));
% round_robin(:,:) = reshape(separated_results(strcmp(types, "rr"),:,:), entry_len,data_len);
% multi_level_feedback_queue = reshape(separated_results(strcmp(types, "mlfq"),:,:),entry_len,data_len);
% 
% L=1;
% B = [];
% for i=1:length(round_robin) 
%     if round_robin(i,1)~="0" || round_robin(i,2)~="0" || round_robin(i,3)~="0"
%         B(L,:)=round_robin(i,:);
%         L=L+1;
%     end
% end
% round_robin = B;
% 
% L=1;
% B = [];
% for i=1:length(multi_level_feedback_queue) 
%     if multi_level_feedback_queue(i,1)~="0" || multi_level_feedback_queue(i,2)~="0" || multi_level_feedback_queue(i,3)~="0"
%         B(L,:)=multi_level_feedback_queue(i,:);
%         L=L+1;
%     end
% end
% multi_level_feedback_queue = B;
% 
% T = table(multi_level_feedback_queue(:,1), multi_level_feedback_queue(:,2), multi_level_feedback_queue(:,3), ...
%     multi_level_feedback_queue(:,4),multi_level_feedback_queue(:,5));
% f = uifigure('Position', [100 100 400 368]);
% uitable(f, 'Data', T, ...
%     'ColumnName', {"Case (Quantum)", "Mean", "Standard Deviation", "Context Switch", "Queue Usage"}, ...
%     'RowName', T.Properties.RowNames, ...
%     'Units', 'Normalized', ...
%     'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
% f.Name = "MLFQ";
% 
% T = table(round_robin(:,1), round_robin(:,2), round_robin(:,3), round_robin(:,4));
% f = uifigure('Position', [100 100 400 368]);
% uitable(f, 'Data', T, ...
%     'ColumnName', {"Case (Quantum)", "Mean", "Standard Deviation", "Context Switch"}, ...
%     'RowName', T.Properties.RowNames, ...
%     'Units', 'Normalized', ...
%     'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
% f.Name = "Round Robin";
 
% f = figure();
% ax = axes(f);
% plot(ax,str2double(round_robin(:,1)),str2double(round_robin(:,2)));
% f.Name = "Round Robin";
% 
% f2 = figure();
% ax2 = axes(f2);
% plot(ax2,str2double(multi_level_feedback_queue(:,1)),str2double(multi_level_feedback_queue(:,2)));
% f2.Name = "Mutli Level Feedback Queues";

T = table(names(:), ...
    means(:), ...
    stds(:), ...
    cs_means(:), ...
    queue_usage(:));
f = uifigure('Position', [100 100 400 368]);
uitable(f, 'Data', T, ...
    'ColumnName', {"Case (Quantum)", "Mean", "Standard Deviation", "Context Switch Time", "Queue Usage"}, ...
    'RowName', T.Properties.RowNames, ...
    'Units', 'Normalized', ...
    'Position', [0.05 0.05 0.9 0.9]); % Adjust position and size within the figure
f.Name = "Quantum: " + quantums(quantum);
% exportapp(f, "Tables_" + quantums(quantum) + ".png");