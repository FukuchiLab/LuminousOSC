#pragma once

#include "TrackingPoint.h"
#include <iostream>
#include <vector>

using namespace std;

class Tracker {

private:
    vector<TrackingPoint> points;
    int max_points = 5;
    int max_life = 10;
    int near_distance = 50;
    int recovery_amount = 5;
    int reducing_amount = 1;
    bool is_enabled = true;

    //--------------------------------------------------------------
    int getNearPointIndex(int x, int y, vector<TrackingPoint>& points, int near_dist) {

        if (points.size() == 0) return -1;

        int min_i = -1;
        double min_d = 0;

        for (int i = 0; i < points.size(); i++) {
            double d = points[i].distanceFrom(x, y);
            if (i == 0 || (d < min_d)) {
                min_d = d;
                min_i = i;
            }
        }

        if (min_d >= near_dist) return -1;
        return min_i;
    }

public:

    //--------------------------------------------------------------
    Tracker() {
    }

    //--------------------------------------------------------------
    bool isEnabled() {
        return is_enabled;
    }

    //--------------------------------------------------------------
    void enable() {
        is_enabled = true;
        points.clear();
    }

    //--------------------------------------------------------------
    void disable() {
        is_enabled = false;
        points.clear();
    }

    //--------------------------------------------------------------
    void setMaxPoints(int new_max_points) {
        this->max_points = new_max_points;
        points.clear();
    }

    //--------------------------------------------------------------
    void setMaxLife(int new_max_life) {
        this->max_life = new_max_life;
    }

    //--------------------------------------------------------------
    void setNearDistance(int new_near_distance) {
        this->near_distance = new_near_distance;
    }

    //--------------------------------------------------------------
    void setRecoveryAmount(int new_recovery_amount) {
        this->recovery_amount = new_recovery_amount;
    }

    //--------------------------------------------------------------
    void setReducingAmount(int new_reducing_amount) {
        this->reducing_amount = new_reducing_amount;
    }

    //--------------------------------------------------------------
    vector<TrackingPoint> get() {
        return points;
    }

    //--------------------------------------------------------------
    void set(vector<cv::Point> candidates) {
        // �g���b�L���O���Ȃ��ꍇ ================================================
        if (!is_enabled) {
            points.clear();
            for (int i = 0; i < candidates.size(); i++) {
                if (points.size() < this->max_points) {
                    points.push_back(TrackingPoint(candidates[i].x, candidates[i].y, i, max_life));
                }
                else {
                    return;
                }
            }
            return;
        }

        // �g���b�L���O����ꍇ ===================================================
       
        // ����t���[���Ō��o���ꂽ���ׂĂ̌��_�Q�i�w��̖ʐςɎ��܂���́j�ƁA
        // ���ݕۗL���Ă���_�Q�ipoints�j�Ƃ̂������ŁA�ł��������߂��ғ��m���y�A�����O����B
        // �y�A�����O����Ȃ��������_�́A������ɒB���Ă��Ȃ���ΐ撅���ŐV�����_�Ƃ��Č}����B
        // �A���S���Y���͐�������ĂȂ��̂ŁA�������������Ȃ炱����������

        // �����e�[�u���̏����� dist_table[����][���]
        vector<vector<double>> dist_table;
        dist_table.resize(points.size());
        for (int i = 0; i < dist_table.size(); i++) {
            dist_table[i].resize(candidates.size());
        }

        // �����v�Z�i���̎��_��臒l�����̂��̂͂͂����j
        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < candidates.size(); j++) {
                double d = points[i].distanceFrom(candidates[j].x, candidates[j].y);;
                if (d < near_distance ) {
                    dist_table[i][j] = d;
                }
                else {
                    dist_table[i][j] = -1; // ����
                }
            }
        }

        // ���_�ɂ��āA�y�A�����O���ꂽ���ǂ����̃t���O
        vector<bool> candidate_paired(candidates.size(), false);

        while (true) {
            // �e�[�u�������`�F�b�N���A���ׂ�-1�Ŗ��܂��Ă�悤�Ȃ�I��
            bool all_minus = true;
            for (int i = 0; i < dist_table.size(); i++) {
                for (double d : dist_table[i]) {
                    if (d >= 0) {
                        all_minus = false;
                        break;
                    }
                }
            }
            if (all_minus) break;

            // ������-1�łȂ��E���E�ł��߂������ɂ���g�ݍ��킹��T��
            int min_i = -1;
            int min_j = -1;
            double min_dist = 99999;
            for (int i = 0; i < points.size(); i++) {
                for (int j = 0; j < candidates.size(); j++) {
                    double d = dist_table[i][j];
                    if (d >=0 && d < min_dist) {
                        min_dist = d;
                        min_i = i;
                        min_j = j;
                    }
                }
            }

            // ���������y�A�ɑ΂��鏈��
            if (min_i >= 0 && min_j >= 0) {
                // �����X�V
                points[min_i].updatePosition(candidates[min_j].x, candidates[min_j].y, recovery_amount);
                for (int i = 0; i < dist_table.size(); i++) dist_table[i][min_j] = -1;
                for (int j = 0; j < dist_table[0].size(); j++) dist_table[min_i][j] = -1;
                candidate_paired[min_j] = true;
            }

        }

        // �y�A�����O����ĂȂ��V�����_������ꍇ�A�ǉ��\�ł���΁A�撅���Œǉ��B
        for (int j = 0; j < candidates.size(); j++) {

            // ����ɒB���Ă���ΏI��
            if (points.size() == this->max_points) return;

            // �y�A�����O����ĂȂ��_�ɂ���
            if (!candidate_paired[j]) {
                // �g���Ă��Ȃ��ŏ��̔ԍ��ɂ��邽�߂Ƀ��X�g�̋󂫈ʒu��T��
                auto itr = points.begin();
                int id = 0;
                while (id < points.size()) {
                    if (points[id].getId() != id) break;
                    id++;
                    itr++;
                }
                points.insert(itr, TrackingPoint(candidates[j].x, candidates[j].y, id, max_life));
            }
        }
    }

    //--------------------------------------------------------------
    void update() {
        if (!is_enabled) return;

        auto itr = points.begin();
        for (int i = 0; i < points.size(); i++) {
            // �_�̃��C�t�X�V����
            points[i].reduceLife(reducing_amount);

            // ���C�t��0�ȉ��ł���΁A���̓_�͍폜
            if (points[i].isDead()) {
                points.erase(itr);
            }
            itr++;
        }
    }

};
