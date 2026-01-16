#include "../include/Editor.h"


Editor::Editor(){
    std::string testData =
        "Hello Gap Buffer!\n"
        "This is a demo.\n"
        "Rendering text using WinAPI.\n";

    m_buffer = GapBuffer();
    loadFromString(testData);

}

void Editor::loadFromString(const std::string& text){
    for (size_t i = 0; i < text.size(); i++)
    {
        if(i == 0){
            m_lineStarts.push_back(0);
        }
        if(text[i] == '\r'){
            if(i + 1 < text.size() && text[i + 1] == '\n'){
                m_lineStarts.push_back(i + 2);
                i++;
            }else{
                m_lineStarts.push_back(i + 1);
            }
            m_buffer.insert('\n');
            continue;
        }

        if(text[i] == '\n'){
            m_lineStarts.push_back(i + 1);
        }
        m_buffer.insert(text[i]);
    }
}

std::string Editor::getRow(int row,int size) const{
    if(row < 0 || row >= static_cast<int>(m_lineStarts.size())){
        return "";
    }
    
    size_t startIndex = m_lineStarts[row];

    return m_buffer.getLineFrom(startIndex,size);
}