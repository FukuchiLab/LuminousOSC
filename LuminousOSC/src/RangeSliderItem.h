// �I���W�i���̃R�[�h�iofxHPVPlayer�̃T���v���R�[�h���j
// https://github.com/vjacob/ofxHPVPlayer/blob/master/example-controls/src/RangeSliderItem.h
// 
// �ύX�_�F
// MIN, MAX ���g���Ă���ӏ���std::min(), std::max() �ɒu�������B
// �͈͂������I�����W�F�̑т��A�V�[�N�o�[�̔w�ʂɗ���悤�ɏC���B
// setRangeInOut()��ǉ��B���͂��玩���I�ɊJ�n�ʒu�܂��͏I���ʒu��ݒ�B
// setSelectedRange()��ǉ��B�����W��0�`1.0�Œ��ڐݒ�B


#ifndef RangeSliderItem_h
#define RangeSliderItem_h

#pragma once

#include "ofMain.h"

class RangeSliderItem
{
public:
    RangeSliderItem(float _x, float _y, float _width, float _height, std::string _name)
    {
        name = _name;
        
        draw_pos.x = _x;
        draw_pos.y = _y;
        float rect_x = draw_pos.x - (_width / 2);
        float rect_y = draw_pos.y - (_height / 2);
        hit_zone = ofRectangle(rect_x, rect_y, _width, _height);
        
        b_selected = false;
        b_hovered = false;
        
        range_in = 0.f;
        range_out = 1.f;
    }
    
    RangeSliderItem()
    {
        
    }
    
    void draw()
    {
        ofPushStyle();

        float line_x = hit_zone.x;
        float line_y = hit_zone.y + (hit_zone.height / 2);

        /* Loop range */
        if (range_in > 0.0f || range_out < 1.0f)
        {
            ofSetColor(255, 174, 0, 100);
            float range_start = line_x + (hit_zone.width * range_in);
            float range_end = hit_zone.width * (range_out - range_in);
            ofDrawRectangle(range_start, line_y - 10, range_end, 20);
        }

        if (b_hovered) ofSetColor(ofColor::lightGrey);
        else ofSetColor(ofColor::white);
        
        /* Main transport line */
        ofDrawLine(line_x, line_y, line_x+hit_zone.width, line_y);
        
        /* Cursor */
        ofSetLineWidth(3.0f);
        ofDrawLine(line_x+cur_pos, line_y-10, line_x+cur_pos, line_y+10);
        
        ofPopStyle();
    }
    
    bool inside(const ofVec2f& hit)
    {
        if (hit_zone.inside(hit))
        {
            if (b_selected) return true;
            else
            {
                b_hovered = true;
                return true;
            }
        }
        else
        {
            b_hovered = false;
            return false;
        }
    }
    
    void select()
    {

    }
    
    void unselect()
    {

    }
    
    std::string& getName()
    {
        
    }
    
    void setPlayhead(float _cur_pos)
    {
        cur_pos = _cur_pos*hit_zone.width;
    }
    
    void mouseDown(const ofVec2f& _cursor)
    {
        cur_pos = (_cursor.x - (hit_zone.x + hit_zone.width * range_in)) / ((range_out - range_in) * hit_zone.width);
    }

    void setRangeInOut(float _input) {
        // �͈͂̂����A�������O�����N���b�N����ƁA�J�n�ʒu��ݒ�B����Ȃ�I���ʒu��ݒ�B
        float pos = (_input - hit_zone.x) / hit_zone.width;
        if (pos < (range_in/2 + range_out/2) ) {
            setRangeIn(_input);
        }
        else {
            setRangeOut(_input);
        }
    }


    void setRangeIn(float _in)
    {
        range_in = (_in - hit_zone.x) / hit_zone.width;
        
        if (range_in > range_out)
        {
            //range_in = MAX(0.0f, range_out - 0.1f);
            range_in = std::max(0.0f, range_out - 0.1f);
        }
    }
   
    void setRangeOut(float _out)
    {
        range_out = (_out - hit_zone.x) / hit_zone.width;
        
        if (range_out < range_in)
        {
            //range_out = MIN(1.0f, range_in + 0.1f);
            range_out = std::min(1.0f, range_in + 0.1f);
        }
    }
    
    void clearRange()
    {
        range_in = 0.f;
        range_out = 1.f;
    }
    
    float getValue()
    {
        return cur_pos;
    }
    
    ofVec2f getSelectedRange()
    {
        return ofVec2f(range_in, range_out);
    }

    // �ǉ��������\�b�h
    // �s�N�Z�����W�ł͂Ȃ��A0�`1.0�͈̔͂Œl���w��
    void setSelectedRange(float in, float out) {
        range_in = in;
        range_out = out;
    }
    
    ofRectangle& getRect()
    {
        return hit_zone;
    }
    
private:
    ofVec2f draw_pos;
    ofRectangle hit_zone;
    float cur_pos;
    float range_in;
    float range_out;
    std::string name;
    bool b_hovered;
    bool b_selected;
};


#endif /* RangeSliderItem_h */
