import helpers
import latex
import matplotlib.pyplot as plt
from os.path import join
import yaml

def make_plot(title, filename, data):
    plt.title(title)
    plt.xlabel('threads')
    plt.ylabel('execution time (ms)')
    plt.xticks([1, 2, 3, 4])
    plt.plot(data.keys(), data.values())
    plt.savefig(filename)
    plt.clf()

def make_test_section(number, data):
    o, a, r, c = data['operations'], data['distributions']['add'], data['distributions']['remove'], data['distributions']['contains']
    plt_title = f'Test {number} (op-s: {o}, d-s: add: {a} %, remove: {r} %, c-s: {c} %)'
    make_plot(plt_title, f'report/photo/plot{number}', data['results'])
    plot = latex.LatexPicture(
        f'plot{number}',
        f'График результатов теста {number}',
    )
    table = latex.LatexTable(
        2, 'l', 
        caption=f'Результаты теста {number}',
        caption_pos='bottom', 
        label=f'results{number}',
    )
    table.set_header('Кол-во потоков', 'Время выполнения (мс)')
    for n, t in data['results'].items():
        table.add_row(n, t)
    
    title = f'\\subsection*{{Тест {number}}}'
    desc_paragraph = '\n'.join((
        f'Параметры тестирования:',
        f'',
        f'\\begin{{itemize}}',
        f'    \\item Количество операций: {o}',
        f'    \\item Вероятность выполнения операции \\verb|add|: {a} \\%',
        f'    \\item Вероятность выполнения операции \\verb|remove|: {r} \\%',
        f'    \\item Вероятность выполнения операции \\verb|contains|: {c} \\%',
        f'\\end{{itemize}}',
    ))
    tab_paragraph = f'Результаты тестирования представленны в таблице \\ref{{{table.label}}}'
    plot_paragraph = f'График результатов тестирования представлен на рис.\\ref{{{plot.label}}}'
    result = '\n\n'.join((title, desc_paragraph, tab_paragraph, table.render(), plot_paragraph, plot.render()))
    return result

def make_tests_sections(test_data: dict):
    sections = [make_test_section(number, data) for number, data in test_data.items()]
    return '\n\n'.join(sections)

def load_data(path):
    with open(path, 'r') as f:
        return yaml.load(f, Loader=yaml.Loader)

def main():
    project_dir = '/home/kirill/CLionProjects/2lab-pas-3-2/'

    test_data_path = join(project_dir, 'test_data.yaml')
    test_data = load_data(test_data_path)

    sections = make_tests_sections(test_data)

    report_section_path = join(project_dir, 'report/modules/chapters/test_results.tex')
    with open(report_section_path, 'w') as f:
        f.write(sections)

if __name__ == '__main__':
    main()
