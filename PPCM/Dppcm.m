% 
clc;clear;close all;fclose all;
width  = 1280;
height = 1280;
quantization_step = 10;% corresponding to QP  32  26   28 16  24 10   20  6.5   16  4
    scale=0;
Files = dir(fullfile('C:\\TMC2\\PCQAS\\distortion\\','*.yuv'));
sort_nat_name = sort_nat({Files.name});
LengthFiles = length(Files);

Files3 = dir(fullfile('C:\\TMC2\\PCQAS\\reference\\','*.yuv'));
dppcm=[];
w=1/256*[ 1 4 6 4 1; 4 16 24 16 4; 6 24 36 24 6; 4 16 24 16 4; 1 4 6 4 1 ];
cordis = 17 % The number of distorted point clouds
for nnum = 1:LengthFiles  
   
original_video_name=strcat('C:\TMC2\PCQAS\reference\',Files3(ceil(nnum/cordis)).name); %17
encoded_video_name = strcat('C:\TMC2\PCQAS\distortion\',sort_nat_name{nnum}); 
encoded_video_name
fidorg = fopen(original_video_name,'r');
fidenc = fopen(encoded_video_name, 'r');

% step 1. read the original and encoded frames
    img_org = double(fread(fidorg,[width height],'ubit8')');
    img_enc = double(fread(fidenc,[width height],'ubit8')');
    temp = fread(fidorg,width*height*0.5,'ubit8');
    temp = fread(fidenc,width*height*0.5,'ubit8');
    
    % % step 2. calculated DPPCM and the estimated 
    % %         values by different models
    scale=1;
    [Theta, actual_dssim_our, estimate_dssim_our, rho_curr] = dssim_dmse_model_our(img_org,img_enc,quantization_step,scale);
 
    img_org2 = imresize(img_org,[height/2,width/2],'bilinear');
    img_enc2 = imresize(img_enc,[height/2,width/2],'bilinear'); 
    scale=2;
    [Theta2, actual_dssim_our2, estimate_dssim_our2, rho_curr2] = dssim_dmse_model_our(img_org2,img_enc2,quantization_step,scale);
    
    img_org3 = imresize(img_org2,[height/4,width/4],'bilinear');
    img_enc3 = imresize(img_enc2,[height/4,width/4],'bilinear');
    scale=4;
    [Theta3, actual_dssim_our3, estimate_dssim_our3, rho_curr3] = dssim_dmse_model_our(img_org3,img_enc3,quantization_step,scale);
 dppcm(nnum)=((sum(estimate_dssim_our(:)).^0.1).*(sum(estimate_dssim_our2(:)).^0.6).*(sum(estimate_dssim_our3(:)).^0.3));
  
   fclose all;
    disp(['Current frame index:' num2str(nnum) 10 '.']);
%     keydown = waitforbuttonpress;
end

Files4 = dir(fullfile('C:\\TMC2\\PCQAS\\gdistortion\\','*.yuv'));
sort_nat_name = sort_nat({Files4.name});
LengthFiles = length(Files4);
Files5 = dir(fullfile('C:\\TMC2\\PCQAS\\greference\\','*.yuv'));
gdppcm=[];

for nnum = 1:LengthFiles;  
    
original_video_name=strcat('C:\TMC2\PCQAS\greference\',Files5(ceil(nnum/cordis)).name);

encoded_video_name = strcat('C:\TMC2\PCQAS\gdistortion\',sort_nat_name{nnum}); 

fidorg = fopen(original_video_name,'r');
fidenc = fopen(encoded_video_name, 'r');

% step 1. read the original and encoded frames
    img_org = double(fread(fidorg,[width height],'ubit8')');
    img_enc = double(fread(fidenc,[width height],'ubit8')');
    temp = fread(fidorg,width*height*0.5,'ubit8');
    temp = fread(fidenc,width*height*0.5,'ubit8');
    
    % % step 2. calculated DPPCM and the estimated 
    % %         values by different models
    scale=1;
    [Theta, actual_dssim_our, estimate_dssim_our, rho_curr] = dssim_dmse_model_our(img_org,img_enc,quantization_step,scale);
    img_org2 = imresize(img_org,[height/2,width/2],'bilinear');
    img_enc2 = imresize(img_enc,[height/2,width/2],'bilinear'); 
    scale=2;
    [Theta2, actual_dssim_our2, estimate_dssim_our2, rho_curr2] = dssim_dmse_model_our(img_org2,img_enc2,quantization_step,scale);
    
    img_org3 = imresize(img_org2,[height/4,width/4],'bilinear');
    img_enc3 = imresize(img_enc2,[height/4,width/4],'bilinear'); 
    scale=4;
    [Theta3, actual_dssim_our3, estimate_dssim_our3, rho_curr3] = dssim_dmse_model_our(img_org3,img_enc3,quantization_step,scale);
 gdppcm(nnum)=((sum(estimate_dssim_our(:)).^0.1).*(sum(estimate_dssim_our2(:)).^0.6).*(sum(estimate_dssim_our3(:)).^0.3));
   fclose all;
    disp(['Current frame index:' num2str(nnum) 10 '.']);
end

save('Dppcm.mat','dppcm','gdppcm')
