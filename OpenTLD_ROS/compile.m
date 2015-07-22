% Copyright 2011 Zdenek Kalal
%
% This file is part of TLD.
%
% TLD is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% TLD is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with TLD.  If not, see <http://www.gnu.org/licenses/>.

% Compiles mex files
clc; clear all; cd mex;

% if ispc
%     disp('PC');
%     include = ' -Ic:\OpenCV2.2\include\opencv\ -Ic:\OpenCV2.2\include\';
%     libpath = 'c:\OpenCV2.2\lib\';
%     files = dir([libpath '*.lib']);
%     
%     lib = [];
%     for i = 1:length(files),
%         lib = [lib ' ' libpath files(i).name];
%     end
%     
%     eval(['mex lk.cpp -O' include lib]);
%     mex -O -c tld.cpp
%     mex -O fern.cpp tld.obj
%     mex -O linkagemex.cpp
%     mex -O bb_overlap.cpp
%     mex -O warp.cpp
%     mex -O distance.cpp
% end
% 
% if ismac
%     disp('Mac');n
%     
%     include = ' -I/opt/local/include/opencv/ -I/opt/local/include/'; 
%     libpath = '/opt/local/lib/'; 
%     
%     files = dir([libpath 'libopencv*.dylib']);
%     
%     lib = [];
%     for i = 1:length(files),
%         lib = [lib ' ' libpath files(i).name];
%     end
%     
%     eval(['mex lk.cpp -O' include lib]);
%     mex -O -c tld.cpp
%     mex -O fern.cpp tld.o
%     mex -O linkagemex.cpp
%     mex -O bb_overlap.cpp
%     mex -O warp.cpp
%     mex -O distance.cpp
%     
% end
%-L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann  

if isunix
    disp('Unix');
   include = ' -I/usr/local/include/opencv/ -I/usr/local/include/ '; 
   %include = ' -I/usr/local/include/opencv/ -I/usr/local/include/ ';
   %libpath = ' -L/usr/local/lib ';
  % [status,include] = system(' pkg-config --cflags opencv ');
 %    [status,libpath] = system('pkg-config --libs opencv');
libpath = ' -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann';     
%      files = dir([libpath 'libopencv*.so']);
%      
%      lib = [];
%      for i = 1:length(files),
%         lib = [lib ' ' libpath files(i).name];
%      end
%      
    %eval(['mex lk.cpp -O -I/usr/local/include/opencv/ -I/usr/local/include/ -L/usr/local/lib/ -L/usr/local']);
    eval(['mex -O lk.cpp ' include libpath]); 
    
    mex -O -c tld.cpp
    mex -O fern.cpp tld.o
    mex -O linkagemex.cpp
    mex -O bb_overlap.cpp
    mex -O warp.cpp
    mex -O distance.cpp
  %  mex -O lk.cpp -Iml -Icv -Icxcore
    
end


cd ..
disp('Compilation finished.');
