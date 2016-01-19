% SVM Classifier
% We will run the SVM linear classifier on each class 
clear;
clc;
curClass = 1;
for curClass=1:4
    if curClass == 1
        trainingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTrainingFeatureMatrix_1.txt');
        testingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTestingFeatureMatrix_1.txt');
    end
    if curClass == 2
        trainingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTrainingFeatureMatrix_2.txt');
        testingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTestingFeatureMatrix_2.txt');
    end
    if curClass == 3
        trainingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTrainingFeatureMatrix_3.txt');
        testingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTestingFeatureMatrix_3.txt');
    end
    if curClass == 4
        trainingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTrainingFeatureMatrix_4.txt');
        testingFeatures = importdata('/home/ravi/Documents/workspace/DisplayImage/formattedTestingFeatureMatrix_4.txt');
    end
    
    
    trainingLabels=zeros(72,1);
    testingLabels=zeros(48,1);

    % The first 36 samples in trainingFeatures are positive samples
    % and reamining are negative. Also, the first 12 samples in 
    % testingFeatures are positive samples and remaing are negative
    % Thus we initialize the label matrices suitably

    for i=1:72
        if(i<=36)
            trainingLabels(i,1)=1;
        else
            trainingLabels(i,1)=-1;
        end
    end
    for i=1:48
        if i<=12
            testingLabels(i,1)=1;
        else
            testingLabels(i,1)=-1;
        end
    end

    model = svmtrain(trainingLabels,trainingFeatures,'-s 0 -t 0');
    fprintf('\nPerformance on Class %d Training Data\n',curClass);
    [predicted_label,accuracy,~]=svmpredict(trainingLabels,trainingFeatures,model);
    fprintf('\nPerformance on Class %d Testing Data\n',curClass);
    [predicted_label,accuracy,~]=svmpredict(testingLabels,testingFeatures,model);
    fprintf('=============================================================\n');
    
end

%%
