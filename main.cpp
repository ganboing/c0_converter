#include <csignal>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <vector>
#include <utility>
#include <boost/regex.hpp>

namespace i0 {
const boost::regex c0_identifier("([a-zA-Z_][a-zA-Z_0-9]*)");
const boost::regex c0_comma(";");
const std::string i0_keyword_standalone("standalone");
const std::string i0_keyword_runner("runner");
const std::string i0_keyword_using("using");
const std::string i0_keyword_watching("watching");
const std::string i0_keyword_abort("abort");
const std::string i0_keyword_abortd("abortd");
const std::string i0_keyword_commit("commit");
const std::string i0_keyword_commitd("commitd");
const std::string i0_keyword_register("register");
const std::string native_standalone("__attribute__((aligned(0x1000)))");
const std::string native_abort(
		"do{void l0_syscall_abort(void) __attribute__((noreturn)); l0_syscall_abort();}while(0)");
const std::string native_abortd(
		"do{void l0_syscall_abortd(void) __attribute__((noreturn)); l0_syscall_abortd();}while(0)");
const std::string native_commit(
		"do{void l0_syscall_commit(void) __attribute__((noreturn)); l0_syscall_commit();}while(0)");
const std::string native_commitd(
		"do{void l0_syscall_commitd(void) __attribute__((noreturn)); l0_syscall_commitd();}while(0)");
typedef std::map<std::string, std::string> i0_str_map_type;
i0_str_map_type str_replace_map;
void init() __attribute__((constructor));
void init() {
	str_replace_map[i0_keyword_abort] = native_abort;
	str_replace_map[i0_keyword_abortd] = native_abortd;
	str_replace_map[i0_keyword_commit] = native_commit;
	str_replace_map[i0_keyword_commitd] = native_commitd;
	str_replace_map[i0_keyword_standalone] = native_standalone;
	str_replace_map[i0_keyword_register] = "";
}
}

typedef std::string::const_iterator str_cit;
typedef str_cit::difference_type str_cit_diff_t;
typedef std::vector<str_cit_diff_t> line_vect_t;
typedef line_vect_t::const_iterator line_vect_cit;
typedef line_vect_cit::difference_type line_vect_cit_diff_t;
typedef std::pair<line_vect_cit_diff_t, str_cit_diff_t> file_position_t;
typedef std::pair<std::string, std::string> pstrstr;
typedef std::pair<str_cit, str_cit> pcitcit;

static line_vect_t file_line_info;
static str_cit c0_src_begin;
static str_cit c0_src_end;

static void build_line_number_vector(const str_cit& begin,
		const str_cit end, line_vect_t& vec) {
	vec.push_back(str_cit_diff_t(0));
	for (str_cit cursor = begin; cursor != end; ++cursor) {
		if (*cursor == '\n') {
			vec.push_back(cursor - begin + 1);
		}
	}
	vec.resize(vec.size());
}

static file_position_t find_pos_in_file(const line_vect_t& vec,
		str_cit_diff_t file_offset) {
	line_vect_cit nxt_line_it = std::upper_bound(vec.begin(), vec.end(),
			file_offset);
	line_vect_cit current_line_it = nxt_line_it - 1;
	line_vect_cit_diff_t line_number = current_line_it - vec.begin() + 1;
	str_cit_diff_t line_offset = file_offset - *(current_line_it) + 1;
	return file_position_t(line_number, line_offset);
}

class parser_error: public std::exception {
public:
	parser_error(const char* what, const str_cit& i) throw () {
		std::stringstream buf;
		file_position_t pos_info = find_pos_in_file(file_line_info,
				i - c0_src_begin);
		buf << what << pos_info.first << " ," << pos_info.second << std::endl;
		msg = buf.str();
		raise(SIGTRAP);
	}
	virtual const char* what() const throw () {
		return msg.c_str();
	}
	virtual ~parser_error() throw () {

	}
private:
	std::string msg;
};



static bool eat_brackets_forward(str_cit& cursor, const str_cit& end) {
	std::stack<char> level_stk;
	bool cursor_forward = false;
	while (cursor != end) {
		switch (*cursor) {
		case '(':
			level_stk.push(')');
			break;
		case '[':
			level_stk.push(']');
			break;
		case '{':
			level_stk.push('}');
			break;
		case ')':
		case ']':
		case '}':
			if (level_stk.empty() || (level_stk.top() != (*cursor))) {
				throw(parser_error("check your brackets forward", cursor));
			}
			level_stk.pop();
			break;
		default:
			if (level_stk.empty()) {
				return cursor_forward;
			}
			break;
		}
		++cursor;
		cursor_forward = true;
	}
	if (!level_stk.empty()) {
		throw(parser_error("check your brackets forward", cursor));
	}
	return cursor_forward;
}

static bool eat_brackets_backward(str_cit& cursor, const str_cit& begin) {
	std::stack<char> level_stk;
	bool cursor_backward = false;
	while (cursor != begin) {
		switch (*(cursor - 1)) {
		case ')':
			level_stk.push('(');
			break;
		case ']':
			level_stk.push('[');
			break;
		case '}':
			level_stk.push('{');
			break;
		case '(':
		case '[':
		case '{':
			if (level_stk.empty() || (level_stk.top() != (*(cursor - 1)))) {
				throw(parser_error("check your brackets backward", cursor));
			}
			level_stk.pop();
			break;
		default:
			if (level_stk.empty()) {
				return cursor_backward;
			}
			break;
		}
		--cursor;
		cursor_backward = true;
	}
	if (!level_stk.empty()) {
		throw(parser_error("check your brackets backward", cursor));
	}
	return cursor_backward;
}

static str_cit find_next_real_comma(const str_cit& begin, const str_cit& end) {
	std::stack<char> level_stk;
	str_cit cursor = begin;
	while (true) {
		eat_brackets_forward(cursor, end);
		if (cursor == end) {
			break;
		}
		if (*cursor == ',') {
			return cursor;
		}
		++cursor;
	}
	return end;
}

static pstrstr split_addr_size(const str_cit& begin, const str_cit& end) {
	static const boost::regex c0_range_comma("(,\\s*,)");
	boost::smatch find_result;
	if (boost::regex_search(begin, end, find_result, c0_range_comma)) {
		str_cit rcursor = end;
		while (true) {
			str_cit last_rcursor = rcursor;
			if (eat_brackets_backward(rcursor, begin)) {
				if ((find_result[0].first < rcursor)
						|| (find_result[0].second > last_rcursor)) {
					throw parser_error("check meta range", rcursor);
				}
				return pstrstr(std::string("(char*)(") + std::string(begin, rcursor) + std::string(")"),
						std::string("(char*)(&((") + std::string(begin, rcursor)
								+ std::string(")[")
								+ std::string(find_result[0].second,
										last_rcursor) + std::string("))-")
								+ std::string("(char*)(&((")
								+ std::string(begin, rcursor) + std::string(")")
								+ std::string(rcursor, find_result[0].first)
								+ std::string("]))"));
			}
			if (rcursor == begin) {
				throw parser_error("check meta range", rcursor);
			}
			--rcursor;
		}
	}
	return pstrstr(
			std::string(
					std::string("(char*)(&(") + std::string(begin, end)
							+ std::string("))")),
			std::string(
					std::string("sizeof(") + std::string(begin, end)
							+ std::string(")")));

}

static std::list<pstrstr> parse_using_watching_ranges(const str_cit& begin,
		const str_cit& end) {
	std::list<pstrstr> output;
	str_cit cursor = begin;
	if (cursor != end) {
		while (true) {
			str_cit nxt_comma = find_next_real_comma(cursor, end);
			output.push_back(split_addr_size(cursor, nxt_comma));
			if (nxt_comma == end) {
				break;
			}
			cursor = nxt_comma + 1;
		}
	}
	return output;
}

static std::string convert_range_meta_to_struct(const char* name,
		const std::list<pstrstr>& list_of_ranges) {
	std::stringstream buf;
	buf << \
		"struct __attribute__((packed)){"
			"unsigned long cnt;";
	if (!list_of_ranges.empty()) {
		buf <<
			"struct __attribute__((packed)){"
				"char* base;"
				"long len;"
			"} ranges[" << list_of_ranges.size() << "];";
	}
	buf <<
		"} " << name << ";";
	{
		size_t cnt = 0;
		buf <<
		"{" <<
			name << ".cnt = " << list_of_ranges.size() << ";";
		for (std::list<pstrstr>::const_iterator i = list_of_ranges.begin();
				i != list_of_ranges.end(); ++i, ++cnt) {
			buf <<
			name << ".ranges[" << cnt << "].base = (" << i->first << ");" <<
			name << ".ranges[" << cnt << "].len = (" << i->second << ");";
		}
		buf <<
		"}";
	}
	return buf.str();
}

static std::string convert_to_l0_spawn_syscall(const str_cit& begin,
		const str_cit& end) {
	str_cit func_call_begin = begin;
	str_cit func_call_end = end;
	str_cit using_begin = end;
	str_cit using_end = end;
	str_cit watching_begin = end;
	str_cit watching_end = end;
	str_cit* last_end = &func_call_end;
	boost::smatch find_result;
	str_cit cursor = begin;

	while (boost::regex_search(cursor, end, find_result, i0::c0_identifier)) {
		cursor = find_result[0].second;
		std::string word_find(find_result[0].first, find_result[0].second);
		str_cit* current_end;
		if (word_find == i0::i0_keyword_using) {
			using_begin = cursor;
			current_end = &using_end;
		} else if (word_find == i0::i0_keyword_watching) {
			watching_begin = cursor;
			current_end = &watching_end;
		} else {
			continue;
		}
		*last_end = find_result[0].first;
		last_end = current_end;
	}
	std::stringstream buf;
	buf <<
		"do{" <<
			convert_range_meta_to_struct("runner_using", parse_using_watching_ranges(using_begin, using_end)) <<
			convert_range_meta_to_struct("runner_watching",parse_using_watching_ranges(watching_begin, watching_end)) <<
			"extern void l0_syscall_spawn(const char*, const char*,const char*);"
			"extern void l0_newr0(void);"
			"l0_syscall_spawn((char*)l0_newr0, (char*)&runner_using, (char*)&runner_watching);"
			"{" <<
				std::string(func_call_begin, func_call_end) << ";" <<
			"}"
		"}while(0)";
	return buf.str();
}

int main(int argc, char** argv) {
	(void) argc;
	std::ifstream file(argv[1]);
	if (!file) {
		std::cerr << "Failure opening file\n";
		return -1;
	}
	const std::string& file_content = std::string(
			(std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));
	boost::smatch result_outer;
	boost::smatch result_inner;
	c0_src_begin = file_content.begin();
	c0_src_end = file_content.end();
	build_line_number_vector(c0_src_begin, c0_src_end, file_line_info);
	str_cit c0_src_cursor(c0_src_begin);
	try {
		while (boost::regex_search(c0_src_cursor, c0_src_end, result_outer,
				i0::c0_identifier)) {
			std::cout << std::string(c0_src_cursor, result_outer[0].first);
			std::string what_i_find(result_outer[0].first,
					result_outer[0].second);
			i0::i0_str_map_type::const_iterator i = i0::str_replace_map.find(
					what_i_find);
			c0_src_cursor = result_outer[0].second;
			if (i != i0::str_replace_map.end()) {
				std::cout << i->second;
			} else {
				if (what_i_find == i0::i0_keyword_runner) {
					if (boost::regex_search(c0_src_cursor, c0_src_end,
							result_inner, i0::c0_comma)) {
						std::cout
								<< convert_to_l0_spawn_syscall(c0_src_cursor,
										result_inner[0].first) << std::endl;
						c0_src_cursor = result_inner[0].first;
					} else {
						throw parser_error("runner syntax!", c0_src_cursor);
					}
				} else {
					std::cout << result_outer[0];
				}
			}
		}
		std::cout << std::string(c0_src_cursor, c0_src_end);
	} catch (std::exception& e) {
		std::cerr << e.what();
		return -1;
	}
	return 0;
}

