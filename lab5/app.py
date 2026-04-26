import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import glob
import os

st.set_page_config(page_title="Аналіз NOAA Даних", layout="wide")

@st.cache_data
def load_and_clean_data(data_dir="vhi_data"):
    data = []
    if not os.path.exists(data_dir):
        return pd.DataFrame()

    for file in glob.glob(f"{data_dir}/*.csv"):
        prov_id = int(os.path.basename(file).split('_')[2])
        with open(file, 'r') as f:
            for line in f:
                clean_line = line.replace('<tt><pre>', '').replace('</pre></tt>', '').strip()
                if not clean_line or 'year' in clean_line.lower():
                    continue
                parts = [x.strip() for x in clean_line.split(',')]

                if len(parts) >= 7:
                    try:
                        vhi_val = float(parts[6])
                        if vhi_val != -1.00:
                            data.append({
                                'NOAA_ID': prov_id,
                                'Year': int(parts[0]),
                                'Week': int(parts[1]),
                                'SMN': float(parts[2]),
                                'SMT': float(parts[3]),
                                'VCI': float(parts[4]),
                                'TCI': float(parts[5]),
                                'VHI': vhi_val
                            })
                    except ValueError:
                        continue

    df = pd.DataFrame(data)
    if df.empty: return df

    noaa_to_ua = {
        1: 22, 2: 24, 3: 23, 4: 25, 5: 3, 6: 4, 7: 8, 8: 19, 9: 20, 10: 21,
        11: 9, 12: 26, 13: 10, 14: 11, 15: 12, 16: 13, 17: 14, 18: 15, 19: 16,
        20: 27, 21: 17, 22: 18, 23: 6, 24: 1, 25: 2, 26: 7, 27: 5
    }

    ua_names = {
        1: 'Вінницька', 2: 'Волинська', 3: 'Дніпропетровська', 4: 'Донецька', 5: 'Житомирська',
        6: 'Закарпатська', 7: 'Запорізька', 8: 'Івано-Франківська', 9: 'Київська', 10: 'Кіровоградська',
        11: 'Луганська', 12: 'Львівська', 13: 'Миколаївська', 14: 'Одеська', 15: 'Полтавська',
        16: 'Рівненська', 17: 'Сумська', 18: 'Тернопільська', 19: 'Харківська', 20: 'Херсонська',
        21: 'Хмельницька', 22: 'Черкаська', 23: 'Чернівецька', 24: 'Чернігівська', 25: 'АР Крим',
        26: 'м. Київ', 27: 'м. Севастополь'
    }

    df['Province_ID'] = df['NOAA_ID'].map(noaa_to_ua)
    df['Province_Name'] = df['Province_ID'].map(ua_names)
    df = df.drop('NOAA_ID', axis=1)

    df['Date'] = pd.to_datetime(df['Year'].astype(str) + df['Week'].astype(str) + '0', format='%Y%W%w')

    return df

df = load_and_clean_data()

if df.empty:
    st.error("Дані не знайдено! Переконайтеся, що папка 'vhi_data' з CSV файлами знаходиться в тій самій директорії.")
    st.stop()

if 'reset' not in st.session_state:
    st.session_state.reset = False

def reset_filters():
    st.session_state.index_selector = 'VHI'
    st.session_state.province_selector = sorted(df['Province_Name'].unique())[0]
    st.session_state.week_range = (1, 52)
    st.session_state.year_range = (int(df['Year'].min()), int(df['Year'].max()))
    st.session_state.sort_asc = False
    st.session_state.sort_desc = False

col_controls, col_content = st.columns([1, 3])

with col_controls:
    st.header("Фільтри")

    st.button("Скинути всі фільтри", on_click=reset_filters)

    selected_index = st.selectbox(
        "Виберіть індекс:", 
        ['VCI', 'TCI', 'VHI'], 
        key='index_selector'
    )

    provinces = sorted(df['Province_Name'].dropna().unique())
    selected_province = st.selectbox(
        "Виберіть область:", 
        provinces, 
        key='province_selector'
    )

    weeks = st.slider(
        "Інтервал тижнів:", 
        min_value=1, max_value=52, 
        value=(1, 52), 
        key='week_range'
    )

    min_yr, max_yr = int(df['Year'].min()), int(df['Year'].max())
    years = st.slider(
        "Інтервал років:", 
        min_value=min_yr, max_value=max_yr, 
        value=(min_yr, max_yr), 
        key='year_range'
    )

    st.markdown("### Сортування")
    sort_asc = st.checkbox("За зростанням", key='sort_asc')
    sort_desc = st.checkbox("За спаданням", key='sort_desc')

filtered_df = df[
    (df['Province_Name'] == selected_province) &
    (df['Year'] >= years[0]) & (df['Year'] <= years[1]) &
    (df['Week'] >= weeks[0]) & (df['Week'] <= weeks[1])
]

if sort_asc and sort_desc:
    st.sidebar.warning("Увага: Увімкнено обидва чекбокси сортування. Застосовано сортування за зростанням за замовчуванням.")
    filtered_df = filtered_df.sort_values(by=selected_index, ascending=True)
elif sort_asc:
    filtered_df = filtered_df.sort_values(by=selected_index, ascending=True)
elif sort_desc:
    filtered_df = filtered_df.sort_values(by=selected_index, ascending=False)

with col_content:
    st.header(f"Аналіз {selected_index} для області: {selected_province}")

    tab1, tab2, tab3 = st.tabs(["Таблиця даних", "Часовий ряд (Графік 1)", "Порівняння областей (Графік 2)"])

    with tab1:
        st.subheader("Відфільтровані дані")
        st.dataframe(filtered_df[['Year', 'Week', 'SMN', 'SMT', 'VCI', 'TCI', 'VHI', 'Province_Name']])

    with tab2:
        st.subheader(f"Динаміка {selected_index} ({years[0]}-{years[1]})") 
        if not filtered_df.empty:
            fig, ax = plt.subplots(figsize=(10, 5))
            plot_df = filtered_df.sort_values(by=['Year', 'Week'])
            sns.lineplot(data=plot_df, x='Date', y=selected_index, ax=ax, color='blue')
            ax.set_title(f"Зміна {selected_index} з часом (Тижні {weeks[0]}-{weeks[1]})")
            ax.set_xlabel("Час")
            ax.set_ylabel(selected_index)
            plt.xticks(rotation=45)
            st.pyplot(fig)
        else:
            st.warning("Немає даних для відображення за вибраними фільтрами.")

    with tab3:
        st.subheader(f"Порівняння {selected_index} по всіх областях")

        compare_df = df[
            (df['Year'] >= years[0]) & (df['Year'] <= years[1]) &
            (df['Week'] >= weeks[0]) & (df['Week'] <= weeks[1])
        ]

        if not compare_df.empty:
            mean_vals = compare_df.groupby('Province_Name')[selected_index].mean().reset_index()
            mean_vals = mean_vals.sort_values(by=selected_index, ascending=False)

            fig2, ax2 = plt.subplots(figsize=(12, 6))
            colors = ['red' if prov == selected_province else 'lightgray' for prov in mean_vals['Province_Name']]

            sns.barplot(data=mean_vals, x='Province_Name', y=selected_index, palette=colors, ax=ax2)
            ax2.set_title(f"Середній {selected_index} по областях ({years[0]}-{years[1]}, тижні {weeks[0]}-{weeks[1]})")
            ax2.set_xlabel("Область")
            ax2.set_ylabel(f"Середній {selected_index}")
            plt.xticks(rotation=90)

            from matplotlib.patches import Patch
            legend_elements = [Patch(facecolor='red', label=selected_province),
                               Patch(facecolor='lightgray', label='Інші області')]
            ax2.legend(handles=legend_elements)

            st.pyplot(fig2)
        else:
            st.warning("Немає даних для порівняння.")
