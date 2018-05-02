%  Created by Yakup Görür (040130052) on 25/02/2018.
%  Digital Signal Processing Design and Application 2017-2018 Spring
%  Homework 6  (only e and f)
%  Lecturer: Prof. Dr. Bilge Günsel, Research Assistant Ya?mur Sabucu




%---------------------------e---------------------------------------------
%FOR "fark.jpg" 
img   = imread('fark.jpg'); % it is a gray image

%Power of FFT spectrum
    im_fft = fft2(img);
    % power
    im_Pfft = abs(im_fft.^2);
    % log of power, avoid log of zeros
    im_logPfft = log(im_Pfft+eps);
    figure(1); clf
    imagesc(im_logPfft), axis image;
    colorbar
    title('Log of power spectrum')

%mesh plot
    psd = 10*log10(abs(fftshift(fft2(img))).^2 );
    figure(2); clf
    mesh(psd)
    shading interp
    title('shape of the log of centered power spectrum')
    
    save power
 
    
   %-------------------f--------------------------------------------------
    [row, col] = size(img);
   Mean_Power_in_frequency= sum(sum(im_Pfft))  / (row*col)^2;
   display(Mean_Power_in_frequency)
   
   
   