

//#include <iostream>
//#include "google/protobuf/compiler/importer.h"
//
//using google::protobuf::compiler::DiskSourceTree;
//using google::protobuf::compiler::MultiFileErrorCollector;
//using google::protobuf::compiler::Importer;
//using google::protobuf::FileDescriptor;
//using google::protobuf::FieldDescriptor;
//using google::protobuf::Descriptor;
//using google::protobuf::SourceLocation;
//
//class ErrorCollector: public MultiFileErrorCollector {
//public:
//  void AddError(const std::string& filename, int line, int column,
//      const std::string& message) override {
//    // TODO: This does not lock stderr, and does not impact result of execution
//    std::cerr << "Error encountered in file " << filename << " Line: " << line << " Col: " << column << "\"" << message << "\"" << std::endl;
//  }
//  void AddWarning(const std::string& filename, int line, int column,
//      const std::string& message) override {
//    std::cout << "Warning encountered in file " << filename << " Line: " << line << " Col: " << column << "\"" << message << "\"" << std::endl;
//  }
//};
//
//std::string FieldTypeName(const FieldDescriptor * fld_descriptor) {
//  if (FieldDescriptor::Type::TYPE_MESSAGE == fld_descriptor->type()) {
//    return fld_descriptor->message_type()->full_name();
//  }
//  return fld_descriptor->type_name();
//}
//
//int main() {
//  DiskSourceTree source_tree;
//  source_tree.MapPath("", "/Volumes/DevHD/cpp/tell/examples/");
//  ErrorCollector error_collector;
//
//  Importer importer(&source_tree, &error_collector);
//  const FileDescriptor *descriptor = importer.Import("example.proto");
//
//  std::cout << "name: " << descriptor->name() << std::endl;
//  std::cout << "package: " << descriptor->package() << std::endl;
//
//  SourceLocation source_location;
//  descriptor->GetSourceLocation(&source_location);
//
//  std::cout << "Leading comments: " << source_location.leading_comments << std::endl;
//  std::cout << "Trailing comments: " << source_location.trailing_comments << std::endl;
//  std::cout << "Leading detached comments: " << std::endl;
//  for (auto comment: source_location.leading_detached_comments) {
//    std::cout << "\t" << comment << std::endl;
//  }
//
//  std::cout << "message count: " << descriptor->message_type_count() << std::endl;
//
//  for (int msg_idx = 0; msg_idx < descriptor->message_type_count(); msg_idx++) {
//    const Descriptor * msg_descriptor = descriptor->message_type(msg_idx);
//    std::cout << "\tmessage " << msg_idx << " name: "<< msg_descriptor->name() << std::endl;
//    msg_descriptor->GetSourceLocation(&source_location);
//    std::cout << "\tLeading comments: " << std::endl << source_location.leading_comments << std::endl;
//    std::cout << "\tTrailing comments: " << source_location.trailing_comments << std::endl;
//    std::cout << "\tLeading detached comments: " << std::endl;
//    for (auto comment: source_location.leading_detached_comments) {
//      std::cout << "\t\t" << comment << std::endl;
//    }
//
//    for (int fld_index = 0; fld_index < msg_descriptor->field_count(); fld_index++) {
//      const FieldDescriptor * fld_descriptor = msg_descriptor->field(fld_index);
//      std::cout << "\tField name: " << fld_descriptor->name() << " type: " << FieldTypeName(fld_descriptor) << std::endl;
//    }
//  }
//}
//
//
