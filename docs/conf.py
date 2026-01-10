project = 'CHB Notes and Manual'
author = 'Andres26'
release = '0.07-prealpha'

extensions = [] 
templates_path = ['_templates']
exclude_patterns = []

# HTML GEN
html_theme = 'classic'

# LATEX GEN
latex_engine = 'lualatex'

latex_documents = [
    (
        'index',
        'CHB.tex',
        'CHB Documentation and manual',
        'andres26',
        'manual',
    ),
]

latex_elements = {
    'classoptions': ',openany',
    'papersize': 'a4paper',
    'pointsize': '10pt',
}


# CHM GEN
htmlhelp_basename = 'CHB'
