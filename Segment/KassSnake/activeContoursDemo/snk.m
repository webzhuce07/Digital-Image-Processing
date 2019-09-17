%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% snk.m is the GUI frontend for a program which demonstrates the active
% contour (snakes) models for image segmentation. To skip the GUI and look
% at the meat of the code, go to interate.m
%
%To run it
%   1. Type guide on the matlab prompt.
%   2. Click on "Go to Existing GUI"
%   3. Select the snk.fig file in the same directory as this file
%   4. Click the green arrow at the top to launch the GUI
%
%   Once the GUI has been launched, you can use snakes by
%   1. Click on "New Image" and load an input image. Samples image are
%   provided.
%   2. Set the smoothing parameter "sigma" or leave it at its default value
%   and click "Filter". This will smooth the image.
%   3. As soon as you click "Filter", cross hairs would appear and using
%   them and left click of you mouse you can pick initial contour location
%   on the image. A red circle would appead everywhere you click and in
%   most cases you should click all the way around the object you want to
%   segement. The last point must be picked using a right-click in order to
%   stop matlab for asking for more points.
%   4. Set the various snake parameters (relative weights of energy terms
%   in the snake objective function) or leave them with their default value
%   and click "Iterate" button. The snake would appear and move as it
%   converges to its low energy state.
%
% Copyright (c) Ritwik Kumar, Harvard University 2010
%               www.seas.harvard.edu/~rkkumar
%
% This code implements “Snakes: Active Contour Models” by Kass, Witkin and
% Terzopolous incorporating Eline, Eedge and Eterm energy factors. See the
% included report and the paper to learn more.
%
% If you find this useful, also look at Radon-Like Features based
% segmentation in  the following paper:
% Ritwik Kumar, Amelio V. Reina & Hanspeter Pfister, “Radon-Like Features 
% and their Application to Connectomics”, IEEE Computer Society Workshop %
% on Mathematical Methods in Biomedical Image Analysis (MMBIA) 2010
% http://seas.harvard.edu/~rkkumar
% Its code is also available on MATLAB Central
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function varargout = snk(varargin)
% SNK M-file for snk.fig
%      SNK, by itself, creates a new SNK or raises the existing
%      singleton*.
%
%      H = SNK returns the handle to a new SNK or the handle to
%      the existing singleton*.
%
%      SNK('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SNK.M with the given input arguments.
%
%      SNK('Property','Value',...) creates a new SNK or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before snk_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to snk_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help snk

% Last Modified by GUIDE v2.5 15-Jan-2006 20:22:33

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @snk_OpeningFcn, ...
                   'gui_OutputFcn',  @snk_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin & isstr(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before snk is made visible.
function snk_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to snk (see VARARGIN)

% Choose default command line output for snk
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

%default initial values for various parameters
set(handles.edit9,'string','1.00');
set(handles.edit5,'string','0.40');
set(handles.edit6,'string','0.20');
set(handles.edit7,'string','1.00');
set(handles.edit8,'string','0.15');

set(handles.edit11,'string','0.30');
set(handles.edit12,'string','0.40');
set(handles.edit13,'string','0.70');

set(handles.edit10,'string','200');

%Housekeeping
set(handles.edit5,'Enable','off');
set(handles.edit6,'Enable','off');
set(handles.edit7,'Enable','off');
set(handles.edit8,'Enable','off');
set(handles.edit10,'Enable','off');
set(handles.edit11,'Enable','off');
set(handles.edit12,'Enable','off');
set(handles.edit13,'Enable','off');
set(handles.pushbutton2,'Enable','off');

% UIWAIT makes snk wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = snk_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit3_Callback(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit3 as text
%        str2double(get(hObject,'String')) returns contents of edit3 as a double


% --- Executes during object creation, after setting all properties.
function edit4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit4_Callback(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit4 as text
%        str2double(get(hObject,'String')) returns contents of edit4 as a double


% --- Executes during object creation, after setting all properties.
function edit5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit5_Callback(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit5 as text
%        str2double(get(hObject,'String')) returns contents of edit5 as a double


% --- Executes during object creation, after setting all properties.
function edit6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit6_Callback(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit6 as text
%        str2double(get(hObject,'String')) returns contents of edit6 as a double


% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit7_Callback(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit7 as text
%        str2double(get(hObject,'String')) returns contents of edit7 as a double


% --- Executes during object creation, after setting all properties.
function edit8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit8_Callback(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit8 as text
%        str2double(get(hObject,'String')) returns contents of edit8 as a double


% --------------------------------------------------------------------
function img_Callback(hObject, eventdata, handles)
% hObject    handle to img (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

sigma_val = str2double(get(handles.edit9,'String')); % gets sigma from text box
y = filter_function(handles.image,sigma_val); % smooths the image
axes(handles.axes1)
imshow(y,[]);
[xs, ys] = getsnake(y); % intializing the snake on the image

handles.smth = y;
handles.xs = xs;
handles.ys = ys;
guidata(hObject, handles);

% Housekeeping
set(handles.edit5,'Enable','on');
set(handles.edit6,'Enable','on');
set(handles.edit7,'Enable','on');
set(handles.edit8,'Enable','on');
set(handles.edit9,'Enable','off');
set(handles.edit10,'Enable','on');
set(handles.edit11,'Enable','on');
set(handles.edit12,'Enable','on');
set(handles.edit13,'Enable','on');
set(handles.pushbutton1,'Enable','off');
set(handles.pushbutton2,'Enable','on');


% --- Executes during object creation, after setting all properties.
function edit9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit9_Callback(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit9 as text
%        str2double(get(hObject,'String')) returns contents of edit9 as a double


% --- Executes during object creation, after setting all properties.
function edit10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit10_Callback(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double


% --- Executes during object creation, after setting all properties.
function edit11_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit11_Callback(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit11 as text
%        str2double(get(hObject,'String')) returns contents of edit11 as a double


% --- Executes during object creation, after setting all properties.
function edit12_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit12_Callback(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit12 as text
%        str2double(get(hObject,'String')) returns contents of edit12 as a double


% --- Executes during object creation, after setting all properties.
function edit13_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edit13_Callback(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit13 as text
%        str2double(get(hObject,'String')) returns contents of edit13 as a double


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%obtiaing various paramaeters form the text boxes
alpha_val = str2double(get(handles.edit5,'String'));
beta_val = str2double(get(handles.edit6,'String'));
gamma_val = str2double(get(handles.edit7,'String'));
kappa_val = str2double(get(handles.edit8,'String'));
weline_val = str2double(get(handles.edit11,'String'));
weedge_val = str2double(get(handles.edit12,'String'));
weterm_val = str2double(get(handles.edit13,'String'));
inter_val = str2double(get(handles.edit10,'String'));

% housekeeping
set(handles.pushbutton1,'Enable','off');
set(handles.pushbutton3,'Enable','off');
set(handles.pushbutton4,'Enable','off');

set(handles.pushbutton2,'Enable','off');
% Making the snake move
interate(handles.smth, handles.xs, handles.ys, alpha_val, beta_val, gamma_val, kappa_val, weline_val, weedge_val, weterm_val, inter_val);
set(handles.pushbutton2,'Enable','on');

set(handles.pushbutton1,'Enable','on');
set(handles.pushbutton3,'Enable','on');
set(handles.pushbutton4,'Enable','on');




% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[filename, pathname] = uigetfile(...
    {'*.tif;*.jpg;*.pgm';'*.*'},'File Selector');
handles.filename = strcat(pathname,'\',filename);
guidata(hObject, handles);
handles.filename

axes(handles.axes1)
x = imread(handles.filename);
imshow(x,[]);

handles.image = x;
guidata(hObject, handles);

% Following code set the parameters according to the image selected
if(strcmp(filename,'img1.tif'))
    set(handles.edit9,'string','2.00');
    set(handles.edit5,'string','0.40');
    set(handles.edit6,'string','0.20');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.15');
    
    set(handles.edit11,'string','0.30');
    set(handles.edit12,'string','0.40');
    set(handles.edit13,'string','0.70');
    
    set(handles.edit10,'string','200');
    
elseif(strcmp(filename,'img2.tif'))
    set(handles.edit9,'string','1.00');
    set(handles.edit5,'string','0.40');
    set(handles.edit6,'string','0.20');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.15');
    
    set(handles.edit11,'string','0.30');
    set(handles.edit12,'string','0.40');
    set(handles.edit13,'string','0.70');
    
    set(handles.edit10,'string','300');
    
elseif(strcmp(filename,'square.jpg'))
    set(handles.edit9,'string','1.00');
    set(handles.edit5,'string','0.20');
    set(handles.edit6,'string','0.20');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.1');
    
    set(handles.edit11,'string','0.0');
    set(handles.edit12,'string','0.40');
    set(handles.edit13,'string','0.0');
    
    set(handles.edit10,'string','450');
elseif(strcmp(filename,'circle.jpg'))
    set(handles.edit9,'string','1.00');
    set(handles.edit5,'string','0.40');
    set(handles.edit6,'string','0.20');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.05');
    
    set(handles.edit11,'string','0.00');
    set(handles.edit12,'string','0.40');
    set(handles.edit13,'string','0.00');
    
    set(handles.edit10,'string','200');
elseif(strcmp(filename,'heart.pgm'))
    set(handles.edit9,'string','2.00');
    set(handles.edit5,'string','0.10');
    set(handles.edit6,'string','0.05');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.1');
    
    set(handles.edit11,'string','0.10');
    set(handles.edit12,'string','0.70');
    set(handles.edit13,'string','0.20');
    
    set(handles.edit10,'string','200');
elseif(strcmp(filename,'room.pgm'))
    set(handles.edit9,'string','1.00');
    set(handles.edit5,'string','0.40');
    set(handles.edit6,'string','0.20');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.05');
    
    set(handles.edit11,'string','0.40');
    set(handles.edit12,'string','0.40');
    set(handles.edit13,'string','0.30');
    
    set(handles.edit10,'string','200');
elseif(strcmp(filename,'new.pgm'))
    set(handles.edit9,'string','5.00');
    set(handles.edit5,'string','0.20');
    set(handles.edit6,'string','0.20');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.15');
    
    set(handles.edit11,'string','0.10');
    set(handles.edit12,'string','0.70');
    set(handles.edit13,'string','0.70');
    
    set(handles.edit10,'string','200');
elseif(strcmp(filename,'chest.pgm'))
    set(handles.edit9,'string','1.00');
    set(handles.edit5,'string','0.10');
    set(handles.edit6,'string','0.10');
    set(handles.edit7,'string','1.00');
    set(handles.edit8,'string','0.15');
    
    set(handles.edit11,'string','-0.20');
    set(handles.edit12,'string','0.70');
    set(handles.edit13,'string','0.70');
    
    set(handles.edit10,'string','200');
end

%housekeeping
set(handles.edit5,'Enable','off');
set(handles.edit6,'Enable','off');
set(handles.edit7,'Enable','off');
set(handles.edit8,'Enable','off');
set(handles.edit9,'Enable','on');
set(handles.edit10,'Enable','off');
set(handles.edit11,'Enable','off');
set(handles.edit12,'Enable','off');
set(handles.edit13,'Enable','off');
set(handles.pushbutton2,'Enable','off');
set(handles.pushbutton1,'Enable','on');


% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

axes(handles.axes1)
imshow(handles.image,[]);

% housekeeping
set(handles.edit5,'Enable','off');
set(handles.edit6,'Enable','off');
set(handles.edit7,'Enable','off');
set(handles.edit8,'Enable','off');
set(handles.edit9,'Enable','on');
set(handles.edit10,'Enable','off');
set(handles.edit11,'Enable','off');
set(handles.edit12,'Enable','off');
set(handles.edit13,'Enable','off');
set(handles.pushbutton2,'Enable','off');
set(handles.pushbutton1,'Enable','on');



