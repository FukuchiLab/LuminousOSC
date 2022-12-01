#include "ofApp.h"

// �����F
// Mac���ł́A�V�[�N���삪���������B
// video.setFrame() �Ƀo�O��������ۂ��B


//--------------------------------------------------------------
// �A�v���̊J�n����
void ofApp::setup(){

	// Mac�̏ꍇ�Adata�t�H���_�̃p�X��ύX
#ifdef __APPLE__
    ofSetDataPathRoot("../Resources/data/");
#endif
        
	// �\���摜�̏�����
	// Mat����̕ϊ����Ɏ����I�ɏ������E�T�C�Y�ύX�͂���邪�A
	// �r�f�I�������J�n�����O�ɕ\�����s����̂Ŏ��O�ɏ���������
	view_img.allocate(640, 480, OF_IMAGE_COLOR);

	// GUI�̏���E������A�g���b�L���O�̓����p�����[�^�Ȃǂ�advanced.xml����ǂݍ��ށB�Ȃ��Ȃ�쐬�B
	if (!adv_set.load("advanced.xml")) {
		adv_set.setValue("MinArea_min", DEFAULT_MIN_AREA_MIN);
		adv_set.setValue("MinArea_max", DEFAULT_MIN_AREA_MAX);
		adv_set.setValue("MaxArea_min", DEFAULT_MAX_AREA_MIN);
		adv_set.setValue("MaxArea_max", DEFAULT_MAX_AREA_MAX);
		adv_set.setValue("MaxPoints_min", DEFAULT_MAX_POINTS_MIN);
		adv_set.setValue("MaxPoints_max", DEFAULT_MAX_POINTS_MAX);
		adv_set.setValue("MatchingDistance_min", DEFAULT_MATCHING_DIST_MIN);
		adv_set.setValue("MatchingDistance_max", DEFAULT_MATCHING_DIST_MAX);
		adv_set.setValue("MaxLife", DEFAULT_MAX_LIFE);
		adv_set.setValue("LifeRecoveryAmount", DEFAULT_LIFE_RECOVERY_AMOUNT);
		adv_set.setValue("LifeReducingAmount", DEFAULT_LIFE_REDUCING_AMOUNT);
		adv_set.setValue("ShowConsoleWindow", DEFAULT_SHOW_CONSOLE_WINDOW);
		adv_set.saveFile("advanced.xml");
	}
	tracker.setMaxLife(adv_set.getValue("MaxLife", DEFAULT_MAX_LIFE));
	tracker.setRecoveryAmount(adv_set.getValue("LifeRecoveryAmount", DEFAULT_LIFE_RECOVERY_AMOUNT));
	tracker.setReducingAmount(adv_set.getValue("LifeReducingAmount", DEFAULT_LIFE_REDUCING_AMOUNT));
	isShowConsole = adv_set.getValue("ShowConsoleWindow", DEFAULT_SHOW_CONSOLE_WINDOW);


#ifdef TARGET_WIN32
	// �R���\�[���\��
	if (isShowConsole) {
		AllocConsole();
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONIN$", "r", stdin);
	}
#endif

	// ���X�i�[�̃Z�b�g�A�b�v
	min_area.addListener(this, &ofApp::minAreaChanged);
	max_area.addListener(this, &ofApp::maxAreaChanged);
	max_points.addListener(this, &ofApp::maxPointsChanged);
	show_input.addListener(this, &ofApp::showInputChanged);
	show_binary.addListener(this, &ofApp::showBinaryChanged);
	use_tracking.addListener(this, &ofApp::useTrackingChanged);
	cam_id.addListener(this, &ofApp::camIdChanged);
	cam_width.addListener(this, &ofApp::camWidthChanged);
	cam_height.addListener(this, &ofApp::camHeightChanged);
	cam_fps.addListener(this, &ofApp::camFpsChanged);
	cam_settings_button.addListener(this, &ofApp::camSettingsButtonPressed);
	osc_ip_address.addListener(this, &ofApp::oscIpAddressChanged);
	osc_port.addListener(this, &ofApp::oscPortChanged);
	load_video_file_button.addListener(this, &ofApp::loadFileButtonPressed);
	switch_to_camera_button.addListener(this, &ofApp::switchToCameraButtonPressed);

	// �t�H���g�̃��[�h
	// gui_font_small.load(GUI_FONT_PATH, 9); // �V�[�N�o�[�̐������p
	gui_font_large.load(GUI_FONT_PATH, 20); // No Tracking �\���p

	setCameraMode();
	setupMainUI();
}


//--------------------------------------------------------------
// ���C����UI�̍쐬����
void ofApp::setupMainUI() {
	bMainUIInitializing = true; // ���̃t���O�ɂ���āAGUI���������ɃR�[���o�b�N�֐������s�����̂�h��

	gui.setup();
	gui.setPosition(660, 20);
	gui.setWidthElements(GUI_WIDTH);
	gui.loadFont(GUI_FONT_PATH, 10);
	gui.add(show_input.set(GUI_LABEL_SHOW_INPUT, true));
	gui.add(show_binary.set(GUI_LABEL_SHOW_BINARY, false));
	gui.add(use_tracking.set(GUI_LABEL_USE_TRACKING, true));
	gui.add(max_points.set(GUI_LABEL_MAX_POINTS, 1, adv_set.getValue("MaxPoints_min", DEFAULT_MAX_POINTS_MIN), adv_set.getValue("MaxPoints_max", DEFAULT_MAX_POINTS_MAX)));
	gui.add(threshold.set(GUI_LABEL_THRESHOLD, 128, 0, 255));
	gui.add(min_area.set(GUI_LABEL_MIN_AREA, 10, adv_set.getValue("MinArea_min", DEFAULT_MIN_AREA_MIN), adv_set.getValue("MinArea_max", DEFAULT_MIN_AREA_MAX)));
	gui.add(max_area.set(GUI_LABEL_MAX_AREA, 500, adv_set.getValue("MaxArea_min", DEFAULT_MAX_AREA_MIN), adv_set.getValue("MaxArea_max", DEFAULT_MAX_AREA_MAX)));
	gui.add(dist_matching.set(GUI_LABEL_DIST_MATCHING, 50, adv_set.getValue("MatchingDistance_min", DEFAULT_MATCHING_DIST_MIN), adv_set.getValue("MatchingDistance_max", DEFAULT_MATCHING_DIST_MAX)));
	gui.add(osc_ip_address.set(GUI_LABEL_OSC_IP_ADDRESS, "127.0.0.1"));
	gui.add(osc_port.set(GUI_LABEL_OSC_PORT, "12000"));
	gui.add(osc_address_pattern.set(GUI_LABEL_OSC_ADDRESS_PATTERN, "/points"));
	if (isCameraMode()) {
		gui.add(cam_id.set(GUI_LABEL_CAM_ID, "0"));
		gui.add(cam_width.set(GUI_LABEL_CAM_WIDTH, "640"));
		gui.add(cam_height.set(GUI_LABEL_CAM_HEIGHT, "480"));
		gui.add(cam_fps.set(GUI_LABEL_CAM_FPS, "30"));
		// �J�����ݒ�̌Ăяo���{�^���͌��݂�Windows�̂ݑΉ�
#ifdef TARGET_WIN32
		gui.add(cam_settings_button.setup(GUI_LABEL_CAM_SETTINGS_BUTTON));	
#endif
	}
	gui.add(load_video_file_button.setup(GUI_LABEL_LOAD_VIDEO_FILE_BUTTON));
	if (isVideoFileMode()) {
		gui.add(switch_to_camera_button.setup(GUI_LABEL_SWITCH_TO_CAMERA_BUTTON));
	}

	// �ݒ�t�@�C������GUI�̐ݒ�l�����[�h
	gui.loadFromFile("settings.xml");

	// �O��l�̏�����
	if (isCameraMode()) {
		prev_cam_id = cam_id.get();
		prev_cam_width = cam_width.get();
		prev_cam_height = cam_height.get();
		prev_cam_fps = cam_fps.get();
	}

	// �X�V���N�G�X�g�t���O
	bRequestChangeOSC = true;
	bRequestChangeCamera = true;
	bRequestChangeMaxPoints = true;
	bRequestChangeUseTracking = true;
}

//--------------------------------------------------------------
// ���C����GUI�̉������
void ofApp::releaseMainUI() {
	gui.clear();
}

//--------------------------------------------------------------
// �r�f�I����UI�̍쐬����
void ofApp::setupVideoUI() {

	MenuItem* play = new MenuItem("ui/button_play.png", 65, ofGetHeight() - 50, "PLAY");
	MenuItem* pause = new MenuItem("ui/button_pause.png", 125, ofGetHeight() - 50, "PAUSE");
	MenuItem* stop = new MenuItem("ui/button_stop.png", 185, ofGetHeight() - 50, "STOP");

	ui_items["PLAY"] = play;
	ui_items["STOP"] = stop;
	ui_items["PAUSE"] = pause;

	int slider_width = max(500, ofGetWidth() - 300);
	range_slider = new RangeSliderItem(250 + slider_width / 2.0, ofGetHeight() - 50, slider_width, 50, "RANGE_SLIDER");

	// �O��̃��[�v�͈͂𕜌�
	range_slider->setSelectedRange(prev_range.x, prev_range.y);
}

//--------------------------------------------------------------
// �r�f�I����UI�̉������
void ofApp::releaseVideoUI() {
	// ����O�Ƀ��[�v�͈͐ݒ���o���Ă���
	prev_range = range_slider->getSelectedRange();
	
	for (auto& item : ui_items) {
		delete(item.second);
		item.second = nullptr;
	}
	ui_items.clear();
	delete(range_slider);
	range_slider = nullptr;
}

//--------------------------------------------------------------
// ���t���[���̍X�V����
void ofApp::update() {
	bMainUIInitializing = false;

	// �������̃p�����[�^�̕ύX�́A���j���[�̃R�[���o�b�N�֐��ł��ƏՓ˂��N���邽�߁A
	// �R�[���o�b�N�֐��ł͗v�X�V�t���O�������ĂāA�����ōX�V�������s���B

	// �r�f�I�t�@�C���ǂݍ���
	if (bRequestChangeVideoFile) {
		if (video.load(video_file_path)) {
			video.play();

			loop_start_frame = 0;
			loop_end_frame = video.getTotalNumFrames() - 1;
			view_img.clear();

			// �t�@�C���ǂݍ��ݐ������_�ŃJ�������[�h��������
			if (isCameraMode()) {
				grabber.close();
				releaseMainUI();
				setVideoFileMode();
				setupMainUI();
				setupVideoUI();
			}
		}
		else {
			view_img.clear();
		}
		bRequestChangeVideoFile = false;
	}

	// �J�������[�h�ւ̐؂�ւ�
	if (bRequestSwitchToCamera) {
		view_img.clear();
		video.close();
		releaseVideoUI();
		releaseMainUI();
		setCameraMode();
		setupMainUI();
		bRequestChangeCamera = true;
		bRequestSwitchToCamera = false;
	}

	// OSC�̃p�����[�^�ύX
	if (bRequestChangeOSC) {
		sender.clear();
		sender.setup(osc_ip_address.get(), ofToInt(osc_port.get()));
		bRequestChangeOSC = false;
	}
	// �J�����̃p�����[�^�ύX
	if (bRequestChangeCamera && isCameraMode()) {
		grabber.close();
		grabber.setDeviceID(ofToInt(cam_id));
		grabber.setup(ofToInt(cam_width), ofToInt(cam_height));
		grabber.setDesiredFrameRate(ofToInt(cam_fps));
		bRequestChangeCamera = false;
	}
	// ���M����_�̍ő吔�̕ύX
	if (bRequestChangeMaxPoints) {
		tracker.setMaxPoints(max_points.get());
		bRequestChangeMaxPoints = false;
	}
	// �g���b�L���O�̗L���̕ύX
	if (bRequestChangeUseTracking) {
		if (use_tracking.get()) {
			tracker.enable();
		}
		else {
			tracker.disable();
		}
		bRequestChangeUseTracking = false;
	}
	tracker.setNearDistance(dist_matching.get());


	// GUI�̈ʒu����
	glm::vec3 guipos = gui.getPosition();
	if (guipos.x < 0) gui.setPosition(10, guipos.y);
	if (guipos.y < 0) gui.setPosition(guipos.x, 10);
	if (guipos.x > ofGetWindowWidth()) gui.setPosition(ofGetWindowWidth() - 50, guipos.y);
	if (guipos.y > ofGetWindowHeight()) gui.setPosition(guipos.x, ofGetWindowHeight() - 50);


	// �ȍ~�̏��������Ȃ����f
	if (isVideoFileMode() && !video.isLoaded()) return;
	if (isCameraMode() && !grabber.isInitialized()) return;


	// �r�f�I�t�@�C�����[�h�̍Đ�����
	if (isVideoFileMode()) {
		// �X���C�_�[����
		range_slider->setPlayhead(video.getCurrentFrame() / (float)video.getTotalNumFrames());

		// ���[�v�Đ�����
		if (video.getCurrentFrame() < loop_start_frame) video.setFrame(loop_start_frame + 1);
		if (video.getCurrentFrame() > loop_end_frame) video.setFrame(loop_start_frame + 1);
	}

	// �L���v�`������
	cv::Mat frame;
	if (isVideoFileMode()) {
		video.update();
		frame = ofxCv::toCv(video.getPixels().getPlane(0));
	}
	else if (isCameraMode()) {
		grabber.update();
		frame = ofxCv::toCv(grabber.getPixels().getPlane(0));
		if (frame.empty()) {
			return;
		}
	}

	// �摜�����i�O���C�X�P�[�����E��l���E���x�����O�E���ʗp�ɃO���C���J���[�ϊ��j
	cv::cvtColor(frame, gray, CV_RGB2GRAY);
	cv::threshold(gray, binary, threshold.get(), 255, cv::THRESH_BINARY);
	cv::Mat stats, centroids;
	int n_label = cv::connectedComponentsWithStats(binary, label, stats, centroids);
	cv::cvtColor(binary, binary, CV_GRAY2RGB);
	cv::Mat result = show_input.get() ? frame : binary;

	// ���x���f�[�^������_���擾�i�����x���l��0���w�i�j
	vector<cv::Point> candidates;	// ���_���X�g
	for (int i = 1; i < n_label; i++) {

		// �ʐςƃo�E���f�B���O�{�b�N�X�̏��
		int* param = stats.ptr<int>(i);
		int area = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
		int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
		int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
		int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
		int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];

		// �d�S���W�̎擾
		double* param_c = centroids.ptr<double>(i);
		int cx = static_cast<int>(param_c[0]);
		int cy = static_cast<int>(param_c[1]);

		// �T�C�Y���͈͓��ł���Ό��_�Ƃ���
		if (min_area.get() <= area && area <= max_area.get()) {

			// ���_���X�g�ւ̒ǉ�
			candidates.push_back(cv::Point(cx, cy));

			// ���_�̕\��
			cv::rectangle(result, cv::Rect(x, y, width, height), cv::Scalar(255, 255, 0), 1);
			cv::circle(result, cv::Point(cx, cy), 3, cv::Scalar(0, 255, 255), -1);
			cv::circle(result, cv::Point(cx, cy), 3, cv::Scalar(0, 0, 0), 1);
		}
	}

	// �g���b�L���O����
	tracker.set(candidates); // ���_�Q���g���b�J�[�ɓn��
	tracker.update();        // �X�V�����i���C�t���Z�j
	auto tr_points = tracker.get();

	// ���ʉ摜�쐬
	for (int i = 0; i < tr_points.size(); i++) {
		int x = tr_points[i].getX();
		int y = tr_points[i].getY();
		int id = tr_points[i].getId();
		cv::circle(result, cv::Point(x, y), 3, cv::Scalar(255, 0, 0), -1);
		cv::circle(result, cv::Point(x, y), 3, cv::Scalar(0, 0, 0), 1);
		cv::putText(result, ofToString(id), cv::Point(x + 15, y + 10), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0), 5);
		cv::putText(result, ofToString(id), cv::Point(x + 15, y + 10), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
	}

	// �\���p�摜���m��
	ofxCv::toOf(result, view_img);
	view_img.update();

	// OSC���b�Z�[�W���M
	ofxOscMessage msg;
	msg.setAddress(osc_address_pattern.get());
	for (TrackingPoint pos : tr_points) {
		msg.addIntArg(pos.getX());
		msg.addIntArg(pos.getY());
		msg.addIntArg(pos.getId());
	}
	sender.sendMessage(msg);
}

//--------------------------------------------------------------
// ���t���[���̕`�揈��
void ofApp::draw() {
	ofBackground(50, 50, 50);

	// �\���摜�̕`��
	view_img.draw(0, 0);

	// �r�f�I�t�@�C������UI�\��
	if (isVideoFileMode()) {
		float y = ofGetHeight() - 100;

		// �l�p�`�Ɛ�
		ofPushStyle();
		ofSetColor(125);
		ofDrawLine(0, y, ofGetWidth(), y);
		ofSetColor(0, 0, 0, 100);
		ofDrawRectangle(0, y, ofGetWidth(), 100);
		ofPopStyle();

		// ���j���[�̕`��
		for (auto& item : ui_items) {
			item.second->draw();
		}

		// �X���C�_�[�̕`��
		range_slider->draw();
		// gui_font_small.drawStringShadowed("To define loop points, press Ctrl \nwhile clicking on the seekbar", 740, ofGetHeight() - 52, ofColor(225));
	}

	// �g���b�L���O�������ɂȂ��Ă���Ƃ��̕����\��
	if (!tracker.isEnabled()) {
		ofPushStyle();
		gui_font_large.drawStringShadowed("No Tracking", 10, 28, ofColor(255, 0, 0));
		ofPopStyle();
	}

	// ���C����GUI�̕\��
	gui.draw();
}

//--------------------------------------------------------------
// �I������
void ofApp::exit() {    
	gui.saveToFile("settings.xml");
	// .removeListener(this, &ofApp::);

#ifdef TARGET_WIN32
	if (isShowConsole) {
		FreeConsole();
	}
#endif
}

//--------------------------------------------------------------
bool ofApp::isVideoFileMode() {
	return isVideoFile;
}

//--------------------------------------------------------------
bool ofApp::isCameraMode() {
	return !isVideoFile;
}

//--------------------------------------------------------------
void ofApp::setVideoFileMode() {
	isVideoFile = true;
}

//--------------------------------------------------------------
void ofApp::setCameraMode() {
	isVideoFile = false;
}

//--------------------------------------------------------------
void ofApp::switchToCameraButtonPressed() {
	if (bMainUIInitializing) return;
	// cout << "**** switchToCameraButtonPressed()" << endl;

	bRequestSwitchToCamera = true;
}

//--------------------------------------------------------------
void ofApp::minAreaChanged(int& value) {
	if (bMainUIInitializing) return;
	// cout << "**** minAreaChanged()" << endl;

	if (value > max_area.get()) {
		max_area.set(value);
	}
}

//--------------------------------------------------------------
void ofApp::maxAreaChanged(int& value) {
	if (bMainUIInitializing) return;
	// cout << "**** maxAreaChanged()" << endl;

	if (value < min_area.get()) {
		min_area.set(value);
	}
}

//--------------------------------------------------------------
void ofApp::maxPointsChanged(int& value) {
	if (bMainUIInitializing) return;
	// cout << "**** maxPointsChanged()" << endl;

	bRequestChangeMaxPoints = true;
}

//--------------------------------------------------------------
void ofApp::useTrackingChanged(bool& value) {
	if (bMainUIInitializing) return;
	// cout << "**** useTrackingChanged()" << endl;

	bRequestChangeUseTracking = true;
}


//--------------------------------------------------------------
void ofApp::camIdChanged(string& value) {
	if (bMainUIInitializing) return;
	// cout << "**** camIdChanged()" << endl;

	int param = ofToInt(value);
	if (param < 0 || param >= grabber.listDevices().size()) param = 0;
	value = ofToString(param);

	if (value != prev_cam_id) {
		prev_cam_id = value;
		bRequestChangeCamera = true;
	}
}

//--------------------------------------------------------------
void ofApp::camWidthChanged(string& value) {
	if (bMainUIInitializing) return;
	// cout << "**** camWidthChanged()" << endl;

	int param = ofToInt(value);
	value = ofToString(param);

	if (param <= 0 || param > 8000) {
		value = prev_cam_width;
	}
	else if (value != prev_cam_width) {
		prev_cam_width = value;
		bRequestChangeCamera = true;
	}
}

//--------------------------------------------------------------
void ofApp::camHeightChanged(string& value) {
	if (bMainUIInitializing) return;
	// cout << "**** camHeightChanged()" << endl;

	int param = ofToInt(value);
	value = ofToString(param);

	if (param <= 0 || param > 5000) {
		value = prev_cam_height;
	}
	else if (value != prev_cam_height) {
		prev_cam_height = value;
		bRequestChangeCamera = true;
	}
}

//--------------------------------------------------------------
void ofApp::camFpsChanged(string& value) {
	if (bMainUIInitializing) return;
	// cout << "**** camFpsChanged()" << endl;

	int param = ofToInt(value);
	value = ofToString(param);

	if (param <= 0) {
		value = prev_cam_fps;
	}
	else if (value != prev_cam_fps) {
		prev_cam_fps = value;
		bRequestChangeCamera = true;
	}
}

//--------------------------------------------------------------
void ofApp::oscIpAddressChanged(string& value) {
	if (bMainUIInitializing) return;
	// cout << "**** oscIpAddressChanged()" << endl;

	if (!isIpAddress(value)) {
		value = prev_osc_ip_address;
	}
	else if (value != prev_osc_ip_address) {
		prev_osc_ip_address = value;
		bRequestChangeOSC = true;
	}
}

//--------------------------------------------------------------
void ofApp::oscPortChanged(string& value) {
	if (bMainUIInitializing) return;
	// cout << "**** oscPortChanged()" << endl;

	int param = ofToInt(value);
	value = ofToString(param);
	if (param < 1 || param > 65535) {
		value = prev_osc_port;
	}
	else if (value != prev_osc_port) {
		prev_osc_port = value;
		bRequestChangeOSC = true;
	}
}

//--------------------------------------------------------------
void ofApp::showInputChanged(bool& value) {
	if (bMainUIInitializing) return;
	// cout << "**** showInputChanged()" << endl;

	show_binary.set(!value);
}

//--------------------------------------------------------------
void ofApp::showBinaryChanged(bool& value) {
	if (bMainUIInitializing) return;
	// cout << "**** showBinaryChanged()" << endl;

	show_input.set(!value);
}

//--------------------------------------------------------------
void ofApp::camSettingsButtonPressed() {
	if (bMainUIInitializing) return;
	// cout << "**** camSettingsButtonPressed()" << endl;

	grabber.videoSettings();
}

//--------------------------------------------------------------
void ofApp::loadFileButtonPressed() {
	if (bMainUIInitializing) return;
	// cout << "**** loadFileButtonPressed()" << endl;

	ofFileDialogResult result = ofSystemLoadDialog("Load video file");
	if (result.bSuccess) {
		video_file_path = result.getPath();
		bRequestChangeVideoFile = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (isVideoFileMode()) {
		ofVec2f mouse(x, y);
		for (auto& item : ui_items) {
			item.second->inside(mouse);
		}
		range_slider->inside(mouse);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (isVideoFileMode()) {
		ofVec2f mouse(x, y);
		if (range_slider->inside(mouse)) {
			inputRangeSlider(mouse);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (!isVideoFileMode()) return;

	// �}�E�X���W
	ofVec2f mouse(x, y);

	// �{�^������
	for (auto& item : ui_items) {
		MenuItem* menu_item = item.second;
		if (menu_item->inside(mouse)) {
			menu_item->select();

			if (menu_item->getName() == "PLAY") {
				if (video.isPaused()) {
					video.setPaused(false);
				}
				else {
					video.play();
				}
			}
			else if (menu_item->getName() == "PAUSE") {
				video.setPaused(true);
			}
			else if (menu_item->getName() == "STOP") {
				video.firstFrame();
				video.setPaused(true);
				video.stop();
				range_slider->clearRange();
				loop_start_frame = 0;
				loop_end_frame = video.getTotalNumFrames() - 1;
			}
			return;
		}
	}

	// �X���C�_�[����
	if (range_slider->inside(mouse)) {
		inputRangeSlider(mouse);
		return;
	}
}

//--------------------------------------------------------------
void ofApp::inputRangeSlider(ofVec2f& mouse) {

	// �}�E�X���͂ɂ���ăJ�[�\���ʒu���X�V
	range_slider->mouseDown(mouse);

	// �R���g���[���L�[�������Ȃ���̎��́A�͈͎w��Ƃ���p�̃V�[�N����
	if (ofGetKeyPressed(OF_KEY_CONTROL)) {
		range_slider->setRangeInOut(mouse.x);
		ofVec2f range = range_slider->getSelectedRange();
		range *= video.getTotalNumFrames();
		if (loop_start_frame != (int)range.x) {
			loop_start_frame = (int)range.x;
			video.setFrame(loop_start_frame + 1);
		}
		loop_end_frame = (int)range.y;
	}
	else {
		// ���ʂ̃V�[�N����
		video.setFrame(loop_start_frame + (int)(range_slider->getValue() * (loop_end_frame - loop_start_frame)));
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (isVideoFileMode()) {
		ui_items["PLAY"]->unselect();
		ui_items["PAUSE"]->unselect();
		ui_items["STOP"]->unselect();
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	// �r�f�I�t�@�C�����[�h���ɁA�r�f�I����UI����蒼��
	if (isVideoFileMode()) {
		releaseVideoUI();
		setupVideoUI();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
}

//--------------------------------------------------------------
// ������𕪊�����֐�
vector<string> split(const string& str, char delim) {
	auto i = 0;
	vector<string> list;
	auto pos = str.find(delim);
	while (pos != string::npos) {
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}
	list.push_back(str.substr(i, str.length()));
	return list;
}

//--------------------------------------------------------------
// �����񂪐��l���𔻒肷��֐�
bool isNumber(const string& str) {
	return str.find_first_not_of("0123456789") == string::npos;
}

//--------------------------------------------------------------
// IP�A�h���X����֐�
bool isIpAddress(const string& value) {

	// localhost ��IP�A�h���X�ƌ��Ȃ�
	if (value == "localhost") return true;

	// '.'�ŕ������A�v�f���S�ł���, �e�v�f���͈͓��̐��l�ł����IP�A�h���X�Ɣ���
	vector<string> params = split(value, '.');
	if (params.size() != 4) {
		return false;
	}
	for (string p : params) {
		if (!isNumber(p) || stoi(p) > 255 || stoi(p) < 0) {
			return false;
		}
	}
	return true;
}
