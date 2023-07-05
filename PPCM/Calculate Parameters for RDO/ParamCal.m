clc;clear;close all;fclose all;
% original_video_name='longdress_ra32_r3_df_GOF0_geometry_1280x1472_10bit_p420.yuv';
% encoded_video_name = 'longdress_ra32_r3_df_GOF0_geometry_rec_1280x1472_10bit_p420.yuv'; % encoded under QP 32, ra
% 
% original_video_name='loot_ra32_r3_df_GOF0_geometry_1280x1280_10bit_p420.yuv';
% encoded_video_name = 'loot_ra32_r3_df_GOF0_geometry_rec_1280x1280_10bit_p420.yuv'; % encoded under QP 32, ra
original_video_name='redandblack_ra32a_r5_df_GOF0_geometry_1280x1392_10bit_p420.yuv';
encoded_video_name = 'redandblack_ra32a_r5_df_GOF0_geometry_rec_1280x1392_10bit_p420.yuv'; % encoded under QP 32, ra
% original_video_name='soldier_ra32a_r5_df_GOF0_geometry_1280x1296_10bit_p420.yuv';
% encoded_video_name = 'soldier_ra32a_r5_df_GOF0_geometry_rec_1280x1296_10bit_p420.yuv'; % encoded under QP 32, ra
% % 
% original_video_name='dancer_ra32_r2_df_GOF0_geometry_2560x2032_10bit_p420.yuv';
% encoded_video_name = 'dancer_ra32_r2_df_GOF0_geometry_rec_2560x2032_10bit_p420.yuv'; % encoded under QP 32, ra
% original_video_name='basketball_ra32_r3_df_GOF0_geometry_2560x2048_10bit_p420.yuv';
% encoded_video_name = 'basketball_ra32_r3_df_GOF0_geometry_rec_2560x2048_10bit_p420.yuv'; % encoded under QP 32, ra


width  = 1280;
height = 1392;
quantization_step = 10;% corresponding to QP  32  26   28 16  24 10   20  6.5   16  4
fidorg = fopen(original_video_name,'r');
fidenc = fopen(encoded_video_name, 'r');
fid = fopen('IQDATAN.bin','wb');
scale=0;


% figure
for frame_idx=1:64
    % step 1. read the original and encoded frames
    img_org = double(fread(fidorg,[width height],'ubit16')');
    img_enc = double(fread(fidenc,[width height],'ubit16')');
    temp = fread(fidorg,width*height*0.5,'ubit16');
    temp = fread(fidenc,width*height*0.5,'ubit16');
    
    % % step 2. calculated Dssim and the estimated 
    % %         values by different models
    if frame_idx==1
        rho = nan;
    else
        rho = rho_prev; % recorded rho from the previous frame
    end
    scale=1;
    [Thetanew, Theta, actual_dssim_our, estimate_dssim_our, rho_curr] = dssim_dmse_model_our(img_org,img_enc,rho,quantization_step,scale);
    rho_prev = rho_curr; 
    
    img_org2 = imresize(img_org,[height/2,width/2],'bilinear');
    img_enc2 = imresize(img_enc,[height/2,width/2],'bilinear'); 
    if frame_idx==1
        rho2 = nan;
    else
        rho2 = rho_prev2; % recorded rho from the previous frame
    end

    scale=2;
    [Thetanew2, Theta2, actual_dssim_our2, estimate_dssim_our2, rho_curr2] = dssim_dmse_model_our(img_org2,img_enc2,rho2,quantization_step,scale);
    rho_prev2 = rho_curr2; 
        
    img_org3 = imresize(img_org,[height/4,width/4],'bilinear');
    img_enc3 = imresize(img_enc,[height/4,width/4],'bilinear'); 
    if frame_idx==1
        rho3 = nan;
    else
        rho3 = rho_prev3; % recorded rho from the previous frame
    end

    scale=4;
    [Thetanew3, Theta3, actual_dssim_our3, estimate_dssim_our3, rho_curr3] = dssim_dmse_model_our(img_org3,img_enc3,rho3,quantization_step,scale);
    rho_prev3 = rho_curr3; 
   
        fwrite(fid,((Theta.^0.1).*(Theta2.^0.6).*(Theta3.^0.3)).','double');
    disp(['Current frame index:' num2str(frame_idx) 10 'Press a key to show the results of next frame']);
%     keydown = waitforbuttonpress;
end
 fclose(fid);

% 
% 
% clc;clear;close all;fclose all;
% original_video_name='dancer_ra32_r2_df_GOF0_texture_2560x2032_8bit_p420.yuv';
% encoded_video_name = 'dancer_ra32_r2_df_GOF0_texture_rec_2560x2032_8bit_p420.yuv'; % encoded under QP 32, ra
% % 
% original_video_name='loot_ra32_r3_df_GOF0_texture_1280x1280_8bit_p420.yuv';
% encoded_video_name = 'loot_ra32_r3_df_GOF0_texture_rec_1280x1280_8bit_p420.yuv'; % encoded under QP 32, ra
original_video_name='redandblack_ra32_r5_df_GOF0_texture_1280x1392_8bit_p420.yuv';
encoded_video_name = 'redandblack_ra32_r5_df_GOF0_texture_rec_1280x1392_8bit_p420.yuv'; % encoded under QP 32, ra
% original_video_name='soldier_r5_ra32_df_GOF0_texture_1280x1296_8bit_p420.yuv';
% encoded_video_name = 'soldier_r5_ra32_df_GOF0_texture_rec_1280x1296_8bit_p420.yuv'; % encoded under QP 32, ra

% original_video_name='basketball_ra32_r3_df_GOF0_texture_2560x2048_8bit_p420.yuv';
% encoded_video_name = 'basketball_ra32_r3_df_GOF0_texture_rec_2560x2048_8bit_p420.yuv'; % encoded under QP 32, ra
% original_video_name='longdress_ra32_r2_df_GOF0_texture_1280x1472_8bit_p420.yuv';
% encoded_video_name = 'longdress_ra32_r2_df_GOF0_texture_rec_1280x1472_8bit_p420.yuv'; % encoded under QP 32, ra


width  = 1280;
height = 1392;
quantization_step = 10;% corresponding to QP  32  26   28 16  24 10   20  6.5   16  4
fidorg = fopen(original_video_name,'r');
fidenc = fopen(encoded_video_name, 'r');
fid = fopen('IQTE.bin','wb');
scale=0;

% figure
for frame_idx=1:64
    % step 1. read the original and encoded frames
    img_org = double(fread(fidorg,[width height],'ubit8')');
    img_enc = double(fread(fidenc,[width height],'ubit8')');
    temp = fread(fidorg,width*height*0.5,'ubit8');
    temp = fread(fidenc,width*height*0.5,'ubit8');
    
    % % step 2. calculated Dssim and the estimated 
    % %         values by different models
    if frame_idx==1
        rho = nan;
    else
        rho = rho_prev; % recorded rho from the previous frame
    end
    scale=1;
    [Thetanew, Theta, actual_dssim_our, estimate_dssim_our, rho_curr] = dssim_dmse_model_our(img_org,img_enc,rho,quantization_step,scale);
    rho_prev = rho_curr; 
           
    img_org2 = imresize(img_org,[height/2,width/2],'bilinear');
    img_enc2 = imresize(img_enc,[height/2,width/2],'bilinear'); 
    if frame_idx==1
        rho2 = nan;
    else
        rho2 = rho_prev2; % recorded rho from the previous frame
    end

    scale=2;
    [Thetanew2, Theta2, actual_dssim_our2, estimate_dssim_our2, rho_curr2] = dssim_dmse_model_our(img_org2,img_enc2,rho2,quantization_step,scale);
    rho_prev2 = rho_curr2; 
   
    img_org3 = imresize(img_org,[height/4,width/4],'bilinear');
    img_enc3 = imresize(img_enc,[height/4,width/4],'bilinear'); 
    if frame_idx==1
        rho3 = nan;
    else
        rho3 = rho_prev3; % recorded rho from the previous frame
    end

    scale=4;
    [Thetanew3, Theta3, actual_dssim_our3, estimate_dssim_our3, rho_curr3] = dssim_dmse_model_our(img_org3,img_enc3,rho3,quantization_step,scale);
    rho_prev3 = rho_curr3; 

        fwrite(fid,((Theta.^0.1).*(Theta2.^0.6).*(Theta3.^0.3)).','double');
    disp(['Current frame index:' num2str(frame_idx) 10 'Press a key to show the results of next frame']);
%     keydown = waitforbuttonpress;
end
 fclose(fid);
