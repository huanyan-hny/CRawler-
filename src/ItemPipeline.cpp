//
// Created by elega on 4/27/17.
//

#include "ItemPipeline.h"



namespace Crawler
{

    void ItemPipeline::handle_items(vector<shared_ptr<Item>>& items )
    {
        for (auto item : items)
        {
            lck.lock();
            process_item(item);
            lck.unlock();
        }

    }
    void ItemPipeline::simple_write(shared_ptr<Item> item)
    {
        boost::filesystem::path tmp = boost::filesystem::unique_path();
        string filename =  tmp.string() + ".txt";
        cout << "writing on " << filename << endl;
        ofstream ofs;
        ofs.open(filename, std::ofstream::out);
        ofs << item->to_string();
        ofs.close();
//			item->to_string();
    }
    void ItemPipeline::simple_file_write(shared_ptr<Item> item)
    {
        for (pair<string, File> file_item : item->data)
        {
            boost::filesystem::path tmp = boost::filesystem::unique_path();
            string filename = "./output/" + tmp.string() + file_item.second.extension;
            ofstream ofs;
            ofs.open(filename, ios::binary | std::ofstream::out);
            ofs.write(file_item.second.data.data(), file_item.second.data.size());
            ofs.close();

        }
    }
    void ItemPipeline::simple_file_write(const File& file,string filename) {
        ofstream ofs;
        string path = "./output/" + string_to_filename(filename) + "." + file.extension;
        ofs.open(path,ios::binary | ios::out);
        ofs.write((char *)&(file.data[0]),file.data.size());
        ofs.close();
    }
    string ItemPipeline::string_to_filename(string input)
    {
        for(int i=0;i<input.size();++i){
            for (int j=0;j<forbiddenChars.size();++j) {
                if (input[i] == forbiddenChars[j])
                    input[i] = '_';
            }
        }
        return input;
    }

}
